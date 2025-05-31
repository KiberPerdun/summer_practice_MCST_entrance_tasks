//
// Created by KiberPerdun on 5/19/25.
//

#include "types.h"
#include "fcntl.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "sys/random.h"

int
main (void)
{
  char filename[1000];
  memset (filename, 'o', sizeof (filename) - 1);
  filename[sizeof (filename) - 1] = '\0';

  if (open ("/bfsafaoot/efi", O_RDONLY) < 0)
    {
      puts (strerror(errno));
      perror ("error");
    }
  if (open (filename, O_RDONLY) < 0)
    {
      puts (strerror(errno));
      perror ("error");
    }
}
