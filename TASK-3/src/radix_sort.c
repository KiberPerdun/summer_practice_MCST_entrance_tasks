//
// Created by KiberPerdun on 6/1/25.
//

#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

u0
radix_sort (line_t *lines, u64 num_lines)
{
  if (lines == NULL || num_lines == 0)
    {
      fprintf (stderr, "Ошибка: пустой массив строк для сортировки.\n");
      return;
    }

  u64 max_length, current_pos;
  line_t *temp_lines;

  max_length = get_max_length (lines, num_lines);
  if ((temp_lines = malloc (sizeof (line_t) * num_lines)) == NULL)
    {
      perror ("malloc");
      exit (EXIT_FAILURE);
    }

  for (u64 pos = max_length; pos > 0; --pos)
    {
      current_pos = pos - 1;
      u64 radix_count[RADIX_BUF] = {0};
      for (u64 i = 0, j; i < num_lines; ++i)
        {
          j = (current_pos < lines[i].size) ? lines[i].str[current_pos] : 0;
          ++radix_count[j];
        }

      for (u64 i = 1; i < RADIX_BUF; ++i)
        radix_count[i] += radix_count[i - 1];

      for (u64 i = num_lines, j, k, z; i > 0; --i)
        {
          j = i - 1;
          z = (current_pos < lines[j].size) ? lines[j].str[current_pos] : 0;
          k = --radix_count[z];
          temp_lines[k] = lines[j];
        }

      memcpy (lines, temp_lines, sizeof (line_t) * num_lines);
    }

  free (temp_lines);
  return;
}

