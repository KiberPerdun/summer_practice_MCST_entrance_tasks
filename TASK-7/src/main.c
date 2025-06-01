//
// Created by KiberPerdun on 5/19/25.
//

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>

#define FILENAME_CHILD  "output_child.txt"
#define FILENAME_PARENT "output_parent.txt"
#define BUFFER_SIZE 8912ull

u0
write_to_file (const u0 *buf, u64 size, const char *filename)
{
  i64 wsize_total, wsize;
  i32 fd;

  fd = open (filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0)
    {
      perror ("open");
      exit (EXIT_FAILURE);
    }

  wsize_total = 0;
  for (; wsize_total < size; )
    {
      wsize = write (fd, buf + wsize, size - wsize_total);
      if (wsize < 0)
        {
          perror ("write");
          close (fd);
          exit (EXIT_FAILURE);
        }
      wsize_total += wsize;
    }

  close (fd);
}

int
main (int argc, char *argv[])
{
  struct stat st;
  u64 fsize;
  pid_t pid;
  i32 inpd;
  u0 *buf;

  if (argc != 2)
    {
      fprintf (stderr,"Использование: %s <file>", argv[0]);
      exit (EXIT_FAILURE);
    }

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

  pid = fork ();
  if (pid < 0)
    {
      perror ("fork");
      munmap (buf, fsize);
      close (inpd);
      exit (EXIT_FAILURE);
    }

  if (0 == pid)
    {
      write_to_file (buf, fsize, FILENAME_CHILD);
      return 1;
    }
  else
    {
      write_to_file (buf, fsize, FILENAME_PARENT);
      wait (NULL);
    }

  munmap (buf, fsize);
  close (inpd);
  return 1;
}
