//
// Created by KiberPerdun on 6/1/25.
//

#ifndef SORT_H
#define SORT_H

#include "types.h"

#define RADIX_BUF 256

typedef struct line {
    char *str;
    u64 size;
} line_t;

u0 radix_sort (line_t *lines, u64 num_lines);
u64 get_max_length (line_t *lines, u64 num_lines);

#endif //SORT_H
