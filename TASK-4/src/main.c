//
// Created by KiberPerdun on 5/19/25.
//

#include "types.h"
#include <getopt.h>
#include <stdio.h>
#include <stdio_ext.h>
#include <string.h>

#define LONG_OPTION_ELBRUS 1

static const char correct_options[] = "Options are correct: ";

int
main (int argc, char *argv[])
{
  i32 opt, longindex = 0;
  char invalid_options[(u8)-1];
  char buf[(u16)-1]; *buf = '\0';
  char *invalid_longoptions[(u8)-1];
  u8 invalid_opts = 0, invalid_longopts = 0;
  static struct option long_options[] =
    {
    {"elbrus", required_argument, NULL, LONG_OPTION_ELBRUS},
    };
  const char *valid_elbrus [] =
    {
      "1c+", "2c+", "2c3", "4c", "8c", "16c", NULL
    };

  setvbuf (stdout, buf, _IOFBF, sizeof (buf));
  fputs (correct_options, stdout);

  opterr = 0;

  while ((opt = getopt_long(argc, argv, "mcst", long_options, &longindex)) != -1)
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
            for ( i32 i = 0; valid_elbrus[i]; ++i)
              {
                if (strcmp (valid_elbrus[i], optarg) == 0)
                  {
                    fputs ("--elbrus=", stdout);
                    fputs (optarg, stdout);
                    putc (',', stdout);
                    putc (' ', stdout);
                    goto switch_for_escape;
                  }
              }
            invalid_longoptions[invalid_longopts++] = argv[optind - 1];

            switch_for_escape:
            break;
          }
        case '?':
          {
            if (optopt)
              invalid_options[invalid_opts++] = optopt;
            else
              invalid_longoptions[invalid_longopts++] = argv[optind - 1];
          }

        default:;
        }
    }

  if (optind < argc)
    {
      fputs ("non-options: ", stdout);
      for ( i32 i = optind; i < argc; ++i)
        {
          fputs (argv[i], stdout);
          putc (',', stdout);
          putc (' ', stdout);
        }
    }

  if (!buf[sizeof (correct_options)])
    {
      buf[__fpending (stdout) - 2] = 0;
      memset (buf, 0, sizeof (correct_options));
      fputs ("Options are incorrect. ", stdout);
    }

  if (invalid_opts > 0 || invalid_longopts > 0)
    {
      u8 i = 0;
      fputs ("incorrect options: ", stdout);
      for ( i = 0; i < invalid_opts; ++i)
        {
          putc (invalid_options[i], stdout);
          putc (',', stdout);
          putc (' ', stdout);
        }

      for ( i = 0; i < invalid_longopts; ++i)
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
