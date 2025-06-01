//
// Created by KiberPerdun on 6/1/25.
//

#include "sort.h"

u64
get_max_length (line_t *lines, u64 num_lines)
{
  u64 max_length = 0;
  for (u64 i = 0; i < num_lines; ++i)
      if (lines[i].size > max_length)
          max_length = lines[i].size;

  return max_length;
}
