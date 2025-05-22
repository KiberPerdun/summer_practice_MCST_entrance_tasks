//
// Created by KiberPerdun on 5/19/25.
//

#include "unix.h"
#include <stdio.h>

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

      char buffer[100];
      ssize_t num_read = recv (fd, buffer, sizeof (buffer) - 1, 0);
      if (num_read > 0)
        {
          buffer[num_read] = '\0';
          printf ("Получено сообщение: %s\n", buffer);
        }

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
