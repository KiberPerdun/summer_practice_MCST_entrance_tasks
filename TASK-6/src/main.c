//
// Created by KiberPerdun on 5/19/25.
//

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>

#include "types.h"

/* man 3type stat man 2 stat */

int
main (int argc, char *argv[])
{
  struct stat sb;

  if (argc != 2)
    {
      fprintf (stderr, "Использование: %s <file>\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  if (stat (argv[1], &sb) < 0)
    {
      perror ("lstat");
      exit (EXIT_FAILURE);
    }

  printf ("* * * Введенный файл: %s * * *\n", argv[1]);

  fputs ("[=+=] Тип файла     -> ", stdout);
  switch (sb.st_mode & S_IFMT)
    {
    case S_IFBLK:  puts ("Блочный девайс");    break;
    case S_IFCHR:  puts ("Символьный девайс"); break;
    case S_IFDIR:  puts ("Директория");        break;
    case S_IFIFO:  puts ("FIFO/pipe");         break;
    case S_IFLNK:  puts ("symlink");           break;
    case S_IFREG:  puts ("Файл");              break;
    case S_IFSOCK: puts ("Сокет");             break;
    default:       puts ("Неизвестно?\n");     break;
    }

  /* В идеале выводить информацию о inode */
  printf ("[=+=] inode number  -> %ju\n", sb.st_ino);
  printf ("[=+=] кол-во ссылок -> %ju\n", sb.st_nlink);
  printf ("[=+=] gid владельца -> %ju\n", sb.st_ino);
  printf ("[=+=] uid владельца -> %p\n", &sb.st_uid);

  printf ("[=+=] размер файла  -> %ju байт\n", sb.st_size);
  printf ("[=+=] размер блока  -> %ju байт\n", sb.st_blksize);
  printf ("[=+=] кол-во блоков -> %ju\n", sb.st_blocks);
  if (sb.st_blocks < sb.st_size / 512 )
    // file has holes. Не придумал как назвать кратко
    printf ("файл имеет дыры");

  if (S_ISCHR (sb.st_mode) || S_ISBLK (sb.st_mode))
    {
      printf ("[=+=] major номер устройства -> %u\n", major (sb.st_rdev));
      printf ("[=+=] minor номер устройства -> %u\n", minor (sb.st_rdev));
    }

  printf ("[=+=] время последнего доступа к файлу    -> %s", ctime (&sb.st_atime));
  printf ("[=+=] время последней модификации файла   -> %s", ctime (&sb.st_mtime));
  printf ("[=+=] время последней смены статуса файла -> %s", ctime (&sb.st_ctime));
}
