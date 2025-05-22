//
// Created by KiberPerdun on 5/22/25.
//

#include "unix.h"

unix_t *
unix_close (unix_t *u)
{
  close (u->fd);
  free (u);

  return u;
}
