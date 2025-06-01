//
// Created by KiberPerdun on 5/19/25.
//

#include "types.h"
#include "fcntl.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include "sys/random.h"

u0
print_error ()
{
  printf ("errno: %d\n", errno);
  printf ("strerror: %s", strerror(errno));
  // printf ("sys_errlist: %s", sys_errlist[errno]);
  perror ("error");
  puts ("\n");
}

int
main (void)
{
  char filename[1000];
  memset (filename, 'o', sizeof (filename) - 1);
  filename[sizeof (filename) - 1] = '\0';

  if (open ("/bfsafaoot/efi", O_RDONLY) < 0)
    print_error ();

  if (open (filename, O_RDONLY) < 0)
    print_error ();

  if (open (filename, O_RDONLY) < 0)
    print_error ();

  if (open ("/tmp", O_WRONLY) < 0)
    print_error ();

  if (open ("/MCST_123", O_CREAT | O_WRONLY, 0664) < 0)
    print_error ();
}
