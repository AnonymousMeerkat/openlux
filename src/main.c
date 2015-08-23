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
#include <math.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#include "color.h"
#include "gamma.h"
#include "kelvin.h"

unsigned short* redgamma;
unsigned short* greengamma;
unsigned short* bluegamma;

void
show_version(void)
{
  puts("openlux v"VERSION_STR);
}

void
show_help(char* argv0)
{
  show_version();
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

int
parse_color(char* arg)
{
  if (!strcmp(arg, "auto"))
    {
      return -1;
    }
  else
    {
      int c = atoi(arg);
      c = color_limit(c);
      return c;
    }
}

const struct option long_options[] = {
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
  int opt_red = -1;
  int opt_green = -1;
  int opt_blue = -1;
  bool opt_identity = 0;

  int c;
  int option_index;

  while (1)
    {
      c = getopt_long(argc, argv, "hVk:r:g:b:i", long_options,
                      &option_index);
      if (c == -1)
        break;

      switch (c)
        {
        case 0:
          break;
        case 'h':
          show_help(argv[0]);
          return 0;

        case 'V':
          show_version();
          return 0;

        case 'i':
          opt_identity = 1;
          break;

        case 'k':
          opt_kelvin = atoi(optarg);
          break;

        case 'r':
          opt_red = parse_color(optarg);
          break;

        case 'g':
          opt_green = parse_color(optarg);
          break;

        case 'b':
          opt_blue = parse_color(optarg);
          break;

        default:
          show_help(argv[0]);
          return 1;
        }
    }


  Display* x_display = XOpenDisplay(NULL);

  if (!x_display)
    {
      fprintf(stderr, "Unable to open display\n");
      return 1;
    }

  int vidmode_maj, vidmode_min;
  int extension_event, extension_error;

  if (!XF86VidModeQueryVersion(x_display, &vidmode_maj, &vidmode_min) ||
      !XF86VidModeQueryExtension(x_display, &extension_event, &extension_error))
    {
      fprintf(stderr, "XF86VidMode not supported on your system\n");
      return 2;
    }

  int x_screen = DefaultScreen(x_display);

  int gamma_ramp_size = -1;
  XF86VidModeGetGammaRampSize(x_display, x_screen, &gamma_ramp_size);
  if (gamma_ramp_size != 1024 && gamma_ramp_size != 256)
    {
      fprintf(stderr, "Your bit depth is not supported\n");
      return 3;
    }

  if (gamma_ramp_size == -1)
    {
      fprintf(stderr, "Unable to find gamma ramp size\n");
      return 4;
    }

#define _malloc_or_die(var, size)               \
  {                                             \
    (var) = malloc(size);                       \
    assert(var);                                \
  }

  _malloc_or_die(redgamma, gamma_ramp_size * 2);
  _malloc_or_die(greengamma, gamma_ramp_size * 2);
  _malloc_or_die(bluegamma, gamma_ramp_size * 2);

#undef _malloc_or_die

  unsigned int color;
  if (!opt_identity)
    {
      color = kelvin_to_rgb(opt_kelvin);

#define _override_color(c) {                            \
        if (opt_##c != -1)                              \
          color = color_set_##c(color, opt_##c);        \
      }

      _override_color(red);
      _override_color(green);
      _override_color(blue);

#undef _override_color

      rgb_to_gamma(color, gamma_ramp_size, redgamma, greengamma, bluegamma);
    }
  else
    {
      identity_gamma(gamma_ramp_size, redgamma, greengamma, bluegamma);
    }

  XF86VidModeSetGammaRamp(x_display, x_screen, gamma_ramp_size,
                          redgamma, greengamma, bluegamma);

  XCloseDisplay(x_display);

  free(redgamma);
  free(greengamma);
  free(bluegamma);

  return 0;
}
