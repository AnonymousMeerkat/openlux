/*
  Copyright (c) 2015 MiJyn

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "backend/backend.h"
#include "log.h"
#include "color.h"
#include "gamma.h"
#include "kelvin.h"


static void
ol_main_version(void)
{
  puts("openlux v"VERSION_STR);
}

static void
ol_main_help(char* argv0)
{
  ol_main_version();
  puts("");
  puts("Usage:");
  printf(" %s [options]\n", argv0);
  puts("");
  puts("Adjusts screen color temperature");
  puts("");
  puts("Options:");
  puts(" -k, --kelvin        color temperature in kelvin (1000-40000, default: 3400)");
  puts(" -r, --red           red color channel (0-255, default: auto)");
  puts(" -g, --green         green color channel (0-255, default: auto)");
  puts(" -b, --blue          blue color channel (0-255, default: auto)");
  puts(" -i, --identity      resets display color");
  puts("");
  puts(" -h, --help          display this help and exit");
  puts(" -V, --version       display version information and exit");
}

static ol_color_byte_t
_ol_main_parse_color(char* arg, ol_color_byte_t cbyte)
{
  if (!strcmp(arg, "auto"))
    {
      return cbyte;
    }

  /* TODO: Add increment/decrement support (+10 -10)
     FIXME: atoi with non-numeric inputs causes undefined behaviour */
  int c = atoi(arg);
  c = OL_COLOR_LIMIT(c);
  return c;
}

static struct option _ol_main_long_options[] = {
  {"kelvin",   required_argument, 0, 'k'},
  {"red",      required_argument, 0, 'r'},
  {"green",    required_argument, 0, 'g'},
  {"blue",     required_argument, 0, 'b'},
  {"identity", no_argument,       0, 'i'},
  {"help",     no_argument,       0, 'h'},
  {"version",  no_argument,       0, 'V'},
  {0, 0, 0, 0}
};

int
main(int argc, char** argv)
{
  int opt_kelvin = 3400;
  char opt_red[256] = "auto";
  char opt_green[256] = "auto";
  char opt_blue[256] = "auto";
  bool opt_identity = 0;

  int c;
  int option_index;

  struct ol_backend_s backend;

  while (1)
    {
      c = getopt_long(argc, argv, "hVk:r:g:b:i", _ol_main_long_options,
                      &option_index);
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          break;

        case 'h':
          ol_main_help(argv[0]);
          return 0;

        case 'V':
          ol_main_version();
          return 0;

        case 'i':
          opt_identity = 1;
          break;

        case 'k':
          opt_kelvin = atoi(optarg);
          break;

        case 'r':
          strcpy(opt_red, optarg);
          break;

        case 'g':
          strcpy(opt_green, optarg);
          break;

        case 'b':
          strcpy(opt_blue, optarg);
          break;

        default:
          ol_main_help(argv[0]);
          return 1;
        }
    }


  if (ol_backend_init(&backend)) {
    OL_LOG_ERR("Unable to load backend");
    return 2;
  }

  struct ol_gamma_s gamma;

  OL_GAMMA_MALLOC(backend.gamma_ramp_size, gamma);

  ol_color_t color;
  if (!opt_identity)
    {
      color = ol_kelvin_rgb(opt_kelvin);

      color = OL_COLOR_INIT(
                            _ol_main_parse_color(opt_red,
                                                 OL_COLOR_RED(color)),
                            _ol_main_parse_color(opt_green,
                                                 OL_COLOR_GREEN(color)),
                            _ol_main_parse_color(opt_blue,
                                                 OL_COLOR_BLUE(color))
                            );

      ol_gamma_rgb(color, backend.gamma_ramp_size, gamma.red,
                   gamma.green, gamma.blue);
    }
  else
    {
      ol_gamma_identity(backend.gamma_ramp_size, gamma.red,
                        gamma.green, gamma.blue);
    }

  backend.set_gamma(&backend, gamma.red, gamma.green, gamma.blue);

  OL_GAMMA_FREE(gamma);

  backend.uninit(&backend);

  return 0;
}
