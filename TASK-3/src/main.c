//
// Created by KiberPerdun on 5/19/25.
//

#include "sort.h"

#include <ctype.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define SORT_RADIX 1

u64
get_sort_type (const char *str)
{
  if (strcmp (str, "radix") == 0)
    return SORT_RADIX;
  else
    {
      fprintf (stderr, "Неизвестный тип сортировки: %s\n", str);
      exit (EXIT_FAILURE);
    }
}

int
main (int argc, char *argv[])
{
  u64 fsize, sort_type, num_lines, line_index, start;
  struct stat st;
  line_t *lines;
  pid_t pid;
  i32 inpd;
  char *buf;

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

  num_lines = 0;
  for (u64 i = 0; i < fsize; ++i)
    {
      if ('\n' == buf[i])
        num_lines++;
    }

  if (fsize > 0 && buf[fsize - 1] != '\n')
    num_lines++;

  if ((lines = malloc (sizeof (line_t) * num_lines)) == NULL)
    {
      perror ("malloc");
      munmap (buf, fsize);
      close (inpd);
      exit (EXIT_FAILURE);
    }

  line_index = start = 0;
  for (u64 i = 0; i < fsize; ++i)
    {
      if ('\n' == buf[i])
        {
          if (i > start && !isspace (*(buf + start)))
            {
              lines[line_index].str = buf + start;
              lines[line_index].size = i - start;
              ++line_index;
            }

          start = i + 1;
        }
    }

  if (start < fsize)
    {
      lines[line_index].str = buf + start;
      lines[line_index].size = fsize - start;
      ++line_index;
    }
  num_lines = line_index;

  /*
  for (i32 i = 0; i < lines[2].size; ++i)
    {
      putc (*(lines[2].str + i), stdout);
    }
*/

  switch (sort_type)
    {
      case (SORT_RADIX):
        {
          radix_sort (lines, num_lines);
        }
    }

  i32 outfd = open ("output.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (outfd < 0)
    {
      perror ("open");
      free (lines);
      munmap (buf, fsize);
      close (inpd);
      exit (EXIT_FAILURE);
    }

  for (u64 i = 0; i < num_lines; ++i)
    {
      write (outfd, lines[i].str, lines[i].size);
      write (outfd, "\n", 1);
    }

  munmap (buf, fsize);
  close (inpd);
}
