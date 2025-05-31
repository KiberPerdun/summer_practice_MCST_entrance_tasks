//
// Created by KiberPerdun on 5/19/25.
//

#include "types.h"
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

#define LONG_OPTION_ELBRUS 1
#define MAX_BUF_SIZE 8192

static const char correct_options[] = "Options are correct: ";

static bool
is_valid_elbrus (const char *optarg)
{
  const char *valid_elbrus[]
      = { "1c+", "2c+", "2c3", "4c", "8c", "16c", NULL };
  for (i32 i = 0; valid_elbrus[i]; ++i)
    if (strcmp (valid_elbrus[i], optarg) == 0)
      return true;

  return false;
}

int
main (int argc, char *argv[])
{
  i32 opt, longindex = 0;
  char invalid_options[MAX_BUF_SIZE] = { 0 };
  char buf[MAX_BUF_SIZE] = { 0 };
  *buf = '\0';
  char *invalid_longoptions[MAX_BUF_SIZE] = { 0 };
  u16 invalid_opts = 0, invalid_longopts = 0;
  static struct option long_options[] = {
    { "elbrus", required_argument, NULL, LONG_OPTION_ELBRUS },
  };

  setvbuf (stdout, buf, _IOFBF, sizeof (buf));
  fputs (correct_options, stdout);

  opterr = 0;

  while ((opt = getopt_long (argc, argv, "mcst", long_options, &longindex))
         != -1)
    {
      switch (opt)
        {
        case 'm':
        case 'c':
        case 's':
        case 't':
          {
            putc (opt, stdout);
            putc (',', stdout);
            putc (' ', stdout);
            break;
          }
        case LONG_OPTION_ELBRUS:
          {
            if (is_valid_elbrus (optarg))
              {
                fputs ("elbrus=", stdout);
                fputs (optarg, stdout);
                putc (',', stdout);
                putc (' ', stdout);
              }
            else if (invalid_longopts < MAX_BUF_SIZE)
              invalid_longoptions[invalid_longopts++] = argv[optind - 1];

            break;
          }
        case '?':
          {
            if (optopt && invalid_opts < MAX_BUF_SIZE)
              invalid_options[invalid_opts++] = optopt;

            else if (invalid_longopts < MAX_BUF_SIZE)
              invalid_longoptions[invalid_longopts++] = argv[optind - 1];
          }

        default:;
        }
    }

  if (!buf[sizeof (correct_options)] || optind < argc || invalid_longopts > 0
      || invalid_opts > 0)
    {
      buf[__fpending (stdout) - 2] = 0;
      memset (buf, 0, sizeof (correct_options));
      fputs ("Options are incorrect. ", stdout);
    }

  if (optind < argc)
    {
      fputs ("non-options: ", stdout);
      for (i32 i = optind; i < argc; ++i)
        {
          fputs (argv[i], stdout);
          putc (',', stdout);
          putc (' ', stdout);
        }
    }

  if (invalid_opts > 0 || invalid_longopts > 0)
    {
      u8 i = 0;
      fputs ("incorrect options: ", stdout);
      for (i = 0; i < invalid_opts; ++i)
        {
          putc (invalid_options[i], stdout);
          putc (',', stdout);
          putc (' ', stdout);
        }

      for (i = 0; i < invalid_longopts; ++i)
        {
          fputs (invalid_longoptions[i], stdout);
          putc (',', stdout);
          putc (' ', stdout);
        }
      buf[__fpending (stdout) - 2] = 0;

      return 0;
    }
  buf[__fpending (stdout) - 2] = 0;

  return 0;
}
