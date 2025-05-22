//
// Created by KiberPerdun on 5/19/25.
//

#include "unix.h"

unix_t *
unix_open (const char *path)
{
  unix_t *u;

  if ((u = calloc (1, sizeof (unix_t))) != NULL)
    {
      if ((u->fd = socket (AF_UNIX, SOCK_STREAM, 0)) < 0)
        {
          close (u->fd);
          return NULL;
        }

      u->addr.sun_family = AF_UNIX;
      strncpy (u->addr.sun_path, path, sizeof (u->addr.sun_path) - 1);
    }

  return u;
}
