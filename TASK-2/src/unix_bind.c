//
// Created by KiberPerdun on 5/19/25.
//

#include "unix.h"

i32
unix_bind (unix_t *u)
{
  unlink (u->addr.sun_path);

  if (bind (u->fd, (struct sockaddr *)&u->addr, sizeof (u->addr)) < 0)
    {
      perror ("bind");
      return -1;
    }
  return 0;
}
