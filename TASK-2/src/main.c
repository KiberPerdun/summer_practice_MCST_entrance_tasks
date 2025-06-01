//
// Created by KiberPerdun on 5/19/25.
//

#include "unix.h"
#include <stdio.h>
#include "io_uring.h"

static char *cur_buf = NULL;
static unix_t *cur_unix = NULL;

u0
sigint_handler (i32 sig)
{
  if (cur_buf)
    free (cur_buf);

  if (cur_unix)
    unix_close (cur_unix);

  exit (0);
}

/*
 * Использовать io_uring мне кажется в моей реализации избыточно ибо я не
 * использую все его фичи. Но в любом случае пусть остается так. Первый
 * коммит имеет реализацию без него и вполне рабочий.
*/

int
main (i32 argc, char *argv[])
{
  unix_t *u = unix_open ("/tmp/1");
  cur_unix = u;

  if (argc > 1 && strcmp (argv[1], "server") == 0)
    {
      puts ("Server start");

      if (unix_bind (u) < 0)
        {
          perror ("unix_bind");
          unix_close (u);
          unlink ("/tmp/1");
          exit (EXIT_FAILURE);
        }

      if (listen (u->fd, 128) < 0)
        {
          perror ("listen");
          unix_close (u);
          unlink ("/tmp/1");
          exit (EXIT_FAILURE);
        }

      i32 fd = accept (u->fd, NULL, NULL);
      if (fd < 0)
        {
          perror ("accept");
          unix_close (u);
          unlink ("/tmp/1");
          exit (EXIT_FAILURE);
        }

      struct io_uring ring;
      if (io_uring_queue_init (8, &ring, 0) < 0)
        {
          perror ("uring");
          close (fd);
          unix_close (u);
          unlink ("/tmp/1");
          exit (EXIT_FAILURE);
        }

      char buffer[256];

      for (;;)
        {
          memset (buffer, 0, sizeof (buffer));

          struct io_uring_sqe *sqe = io_uring_get_sqe (&ring);
          io_uring_prep_recv (sqe, fd, buffer, sizeof (buffer) - 1, 0);

          io_uring_submit (&ring);

          struct io_uring_cqe * cqe;
          if (io_uring_wait_cqe (&ring, &cqe) < 0)
            {
              perror ("cqe");
              io_uring_queue_exit (&ring);
              close (fd);
              unix_close (u);
              unlink ("/tmp/1");
              exit (EXIT_FAILURE);
            }

          if (cqe->res > 0)
            {
              buffer[cqe->res] = '\0';
              printf ("recv: %s", buffer);
            }
          else if (0 == cqe->res)
            {
              puts ("client diconnected");
              io_uring_cqe_seen (&ring, cqe);
              break;
            }

          io_uring_cqe_seen (&ring, cqe);
        }
      io_uring_queue_exit (&ring);

      close (fd);
      unix_close (u);
      unlink ("/tmp/1");
    }
  else
    {
      signal (SIGINT, sigint_handler);

      if (connect (u->fd, (struct sockaddr *)&u->addr, sizeof (u->addr)) < 0)
        {
          perror ("Connect");
          unix_close (u);
          exit (EXIT_FAILURE);
        }

      for (;;)
        {
        char *buf = malloc ((u16)-1);
        cur_buf = buf;

        i64 len = read (0, buf, (u16)-1);
        if (len < 0)
          {
            perror ("read");
            free (buf);
            unix_close (u);
            exit (EXIT_FAILURE);
          }
        else if (len == 0)
          {
            free (buf);
            cur_buf = NULL;
            unix_close (u);
            return 0;
          }

        if (send (u->fd, buf, len, 0) < 0)
          {
            perror ("send");
            free (buf);
            unix_close (u);
            exit (EXIT_FAILURE);
          }

        free (buf);
        cur_buf = NULL;
      }
      unix_close (u);
    }

  return 0;
}
