//
// Created by KiberPerdun on 5/19/25.
//

#include "sort.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

u64
get_sort_type (const char *str)
{
  if (strcmp (str, "radix") == 0)
    return 0;
  else
    {
      fprintf (stderr, "Неизвестный тип сортировки: %s\n", str);
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char *argv[])
{
  u64 fsize, sort_type;
  struct stat st;
  pid_t pid;
  i32 inpd;
  u0 *buf;

  if (argc != 3)
    {
      fprintf (stderr,"Использование: %s <file> <sort_type[radix, ...]>", argv[0]);
      exit (EXIT_FAILURE);
    }
  sort_type = get_sort_type (argv[2]);

  inpd = open (argv[1], O_RDONLY);
  if (inpd < 0)
    {
      perror ("open");
      exit (EXIT_FAILURE);
    }

  if (fstat (inpd, &st) < 0)
    {
      perror ("fstat");
      close (inpd);
      exit (EXIT_FAILURE);
    }

  fsize = st.st_size;
  buf = mmap (NULL, fsize, PROT_READ, MAP_PRIVATE, inpd, 0);
  if (MAP_FAILED == buf)
    {
      perror ("mmap");
      close (inpd);
      exit (EXIT_FAILURE);
    }

  munmap (buf, fsize);
  close (inpd);
}
