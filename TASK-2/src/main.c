//
// Created by KiberPerdun on 5/19/25.
//

#include "unix.h"
#include <stdio.h>
#include "io_uring.h"

int
main (i32 argc, char *argv[])
{
  unix_t *u = unix_open ("/tmp/1");

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

      struct io_uring_sqe *sqe = io_uring_get_sqe (&ring);
      io_uring_prep_recv (sqe, fd, buffer, sizeof (buffer), 0);

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
          printf ("recv: %s", buffer);
        }

      io_uring_cqe_seen (&ring, cqe);
      io_uring_queue_exit (&ring);

      close (fd);
      unix_close (u);
      unlink ("/tmp/1");
    }
  else
    {
      if (connect (u->fd, (struct sockaddr *)&u->addr, sizeof (u->addr)) < 0)
        {
          perror ("Connect");
          unix_close (u);
          exit (EXIT_FAILURE);
        }

      char *buf = malloc ((u16)-1);
      i64 len = read (0, buf, (u16)-1);
      if (len < 0)
        {
          perror ("read");
          unix_close (u);
          exit (EXIT_FAILURE);
        }
      else if (len == 0)
        {
          free (buf);
          unix_close (u);
          return 0;
        }

      if (send (u->fd, buf, len, 0) < 0)
        {
          perror ("send");
          unix_close (u);
          exit (EXIT_FAILURE);
        }

      free (buf);
      unix_close (u);
    }

  return 0;
}
