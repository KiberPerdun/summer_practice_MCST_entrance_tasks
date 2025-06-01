//
// Created by KiberPerdun on 5/19/25.
//

#ifndef UNIX_H
#define UNIX_H

#include "types.h"
#include <errno.h>
#include <net/if.h>
#include <netpacket/packet.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

/* адаптированные методы из libdnet для юникс сокетов */

typedef struct unix_handle
{
  i32 fd;
  struct ifreq ifr;
  struct sockaddr_un addr;
} unix_t;

unix_t *unix_open (const char *path);
unix_t *unix_close (unix_t *u);
i32 unix_bind (unix_t *u);
i64 unix_send (unix_t *u, u8 *buf, u64 len);

#endif // UNIX_H