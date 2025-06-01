//
// Created by KiberPerdun on 5/19/25.
//

#include "sys/random.h"
#include <malloc.h>
#include <stdio.h>

#include "types.h"

/*
 * В принципе выбор типа данных для буффера работает. Ошибка возникает
 * только при выводе массива и ответа на экран. Ну и моя реализация
 * данной задачи не проверяет переполнение буффера результата. Может стать
 * критичным
 */
#define TYPE u16
#define BUFFER_SIZE 1024ull

int
main (void)
{
  TYPE *buf = malloc (BUFFER_SIZE * sizeof (TYPE));
  i64 max_sum, current_sum, li, ri; /* ..., ..., left index, right index */
  i64 max_start, max_end;

  li = ri = 0;
  max_start = max_end = 0;
  getrandom (buf, BUFFER_SIZE * sizeof (TYPE), GRND_NONBLOCK);
  max_sum = current_sum = buf[0];

  for (i64 i = 0; i < BUFFER_SIZE; ++i)
    printf ("%016hx ", buf[i]);

  for (i64 i = 1; i < BUFFER_SIZE; ++i)
    {
      if (buf[i] > buf[i - 1])
        current_sum += buf[i];
      else
        {
          if (current_sum > max_sum)
            {
              max_sum = current_sum;
              max_start = li;
              max_end = i - 1;
            }
          current_sum = buf[i];
          li = i;
        }
    }

  if (current_sum > max_sum)
    {
      max_sum = current_sum;
      max_start = li;
      max_end = BUFFER_SIZE - 1;
    }

  printf ("\n%ld, %ld", max_start, max_end);
  printf ("\nМаксимальная сумма: %016lx\n", (u64) max_sum);

  free (buf);
}
