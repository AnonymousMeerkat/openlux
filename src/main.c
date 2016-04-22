/*
  Copyright (c) 2015 Anonymous Meerkat

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

#include "backend/os/os.h"
#include "backend/video/video.h"
#include "backend/gamma/gamma.h"
#include "backend/time/time.h"
#include "backend/kelvin/kelvin.h"
#include "animate.h"
#include "log.h"
#include "color.h"
#include "gamma.h"


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
  char* helpstr =
    "\n"
    "Adjusts screen color temperature\n"
    "\n"
    "Options:\n"
    " -k, --kelvin          color temperature in kelvin (1000-40000,\n"
    "                           default: 3400)\n"
    " -r, --red             red color channel (0-255, default: auto)\n"
    " -g, --green           green color channel (0-255, default: auto)\n"
    " -b, --blue            blue color channel (0-255, default: auto)\n"
    " -i, --identity        resets display gamma to identity\n"
    "                           (no color bias)\n"
    "\n"
    " -K, --kelvin-backend  kelvin algorithm backend, one of these:\n"
    "                           neilb (default)\n"
    "                           tanner\n"
    "\n"
    " -R, --reset           resets display gamma to last saved gamma\n"
    " -s, --save            saves current gamma (automatically done when\n"
    "                           openlux is run for the first time on boot)\n"
    "\n"
    " -a, --animate         animation time in milliseconds (default: 0)\n"
    " -d, --delay           animation delay per \"frame\" in milliseconds\n"
    "                           (default: 0)\n"
    "\n"
    " -h, --help            display this help and exit\n"
    " -V, --version         display version information and exit\n";
  puts(helpstr);
}

static struct option _ol_main_long_options[] = {
  {"kelvin",         required_argument, 0, 'k'},
  {"red",            required_argument, 0, 'r'},
  {"green",          required_argument, 0, 'g'},
  {"blue",           required_argument, 0, 'b'},
  {"identity",       no_argument,       0, 'i'},
  {"kelvin-backend", required_argument, 0, 'K' },
  {"reset",          no_argument,       0, 'R'},
  {"save",           no_argument,       0, 's'},
  {"animate",        required_argument, 0, 'a'},
  {"delay",          required_argument, 0, 'd'},
  {"help",           no_argument,       0, 'h'},
  {"version",        no_argument,       0, 'V'},
  {0, 0, 0, 0}
};

int
main(int argc, char** argv)
{
  int ret = 0;

  int  opt_kelvin         = 3400;
  char opt_red[256]       = "auto";
  char opt_green[256]     = "auto";
  char opt_blue[256]      = "auto";
  bool opt_identity       = 0;
  int  opt_kelvin_backend = OL_BACKEND_KELVIN_INDEX_NEILB;
  bool opt_save           = 0;
  bool opt_reset          = 0;
  int  opt_anim           = 0;
  int  opt_delay          = 0;

  int c;
  int option_index;

  struct ol_backend_os_s     os_backend;
  struct ol_backend_video_s  video_backend;
  struct ol_backend_gamma_s  gamma_backend;
  struct ol_backend_time_s   time_backend;
  struct ol_backend_kelvin_s kelvin_backend;

  struct ol_color_s current_color = OL_COLOR_WHITE;
#ifdef OL_CMAKE_USE_IOS
  struct ol_color_s default_color;
  struct ol_color_s anim_color;
  FILE* color_file;
#endif

  struct ol_gamma_s default_gamma_value;
  struct ol_gamma_s current_gamma_value;
  struct ol_gamma_s anim_gamma_value;
  struct ol_gamma_s gamma_value;
  FILE* gamma_file;

  ol_time_t start_time;
  ol_time_t current_time;


  /*** Parse arguments ***/

  while (1)
    {
      c = getopt_long(argc, argv, "hVk:r:g:b:iK:Rsa:d:",
                      _ol_main_long_options, &option_index);
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

        case 'R':
          opt_reset = 1;
          break;

        case 's':
          opt_save = 1;
          break;

        case 'K':
          if (!strcmp(optarg, "neilb"))
            opt_kelvin_backend = OL_BACKEND_KELVIN_INDEX_NEILB;
          else if (!strcmp(optarg, "tanner"))
            opt_kelvin_backend = OL_BACKEND_KELVIN_INDEX_TANNER;
          else if (!strcmp(optarg, "flux"))
            opt_kelvin_backend = OL_BACKEND_KELVIN_INDEX_FLUX;
          else
            {
              OL_LOG_ERR("Invalid option for --kelvin-backend: %s",
                         optarg);
              return -1;
            }
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

        case 'a':
          opt_anim = atoi(optarg);
          break;

        case 'd':
          opt_delay = atoi(optarg);
          break;

        default:
          ol_main_help(argv[0]);
          return 1;
        }
    }


  /*** Load backends ***/

  if (ol_backend_os_init(&os_backend, -1, NULL))
    {
      OL_LOG_ERR("Unable to load platform backend");
      return 2;
    }

  if (ol_backend_video_init(&video_backend, -1, NULL))
    {
      OL_LOG_ERR("Unable to load video backend");

      ret = 3;
      goto free_os_backend;
    }

  if (ol_backend_gamma_init(&gamma_backend, video_backend.gamma_index,
                            video_backend.gamma_data))
    {
      OL_LOG_ERR("Unable to load gamma backend");

      ret = 4;
      goto free_video_backend;
    }

  if (ol_backend_time_init(&time_backend, os_backend.time_index,
                           NULL))
    {
      OL_LOG_ERR("Unable to load time backend");
      ret = 5;
      goto free_gamma_backend;
    }

  if (ol_backend_kelvin_init(&kelvin_backend, opt_kelvin_backend,
                             NULL))
    {
      OL_LOG_ERR("Unable to load kelvin backend");
      ret = 6;
      goto free_kelvin_backend;
    }


  OL_GAMMA_MALLOC(video_backend.gamma_ramp_size, current_gamma_value);
  video_backend.get_gamma(&video_backend, current_gamma_value);

  if (!os_backend.exists(&os_backend, "gamma") || opt_save)
    {
      /* Save gamma */
      gamma_file = os_backend.open(&os_backend, "gamma", "wb");
      fwrite(current_gamma_value.red,
             1, OL_GAMMA_SIZE(video_backend.gamma_ramp_size),
             gamma_file);
      fclose(gamma_file);

      if (opt_save)
        goto free_current_gamma_value;
    }


  /*** Load default gamma ***/

  if (gamma_backend.needs_default_gamma || opt_reset)
    {
      OL_GAMMA_MALLOC(video_backend.gamma_ramp_size, default_gamma_value);

      gamma_file = os_backend.open(&os_backend, "gamma", "rb");
      fread(default_gamma_value.red,
            1, OL_GAMMA_SIZE(video_backend.gamma_ramp_size),
            gamma_file);
      fclose(gamma_file);

      gamma_backend.set_default_gamma(&gamma_backend, default_gamma_value);
    }

  OL_GAMMA_MALLOC(video_backend.gamma_ramp_size, gamma_value);

  /*** Load gamma ***/

  if (opt_reset)
    {
      OL_GAMMA_COPY(gamma_value, default_gamma_value,
                    video_backend.gamma_ramp_size);
    }
  else if (!opt_identity)
    {
      current_color = kelvin_backend.kelvin(&kelvin_backend,
                                            opt_kelvin);

      current_color.red   = ol_color_parse(opt_red,
                                           current_color.red);
      current_color.green = ol_color_parse(opt_green,
                                           current_color.green);
      current_color.blue  = ol_color_parse(opt_blue,
                                           current_color.blue);

      gamma_backend.rgb(&gamma_backend,
                        video_backend.gamma_ramp_size,
                        current_color, gamma_value);
    }
  else
    {
      gamma_backend.identity(&gamma_backend,
                             video_backend.gamma_ramp_size, gamma_value);
    }


  /*** Load color ***/

#ifdef OL_CMAKE_USE_IOS
  default_color = OL_COLOR_WHITE;

  if (os_backend.exists(&os_backend, "color"))
    {
      color_file = os_backend.open(&os_backend, "color", "rb");
      fread(&default_color, 1, sizeof(struct ol_color_s), color_file);
      fclose(color_file);
    }
#endif


  /*** Set gamma ***/

  if (opt_anim > 0)
    {
      /* Animate it */

      OL_GAMMA_MALLOC(video_backend.gamma_ramp_size, anim_gamma_value);

      start_time = time_backend.get_time(&time_backend);

      for (current_time = time_backend.get_time(&time_backend) - start_time;
           current_time < opt_anim;
           current_time = time_backend.get_time(&time_backend) - start_time)
        {
#ifndef OL_CMAKE_USE_IOS
          ol_animate_lerp(current_gamma_value, gamma_value, anim_gamma_value,
                          video_backend.gamma_ramp_size,
                          current_time, opt_anim);
#else
          ol_animate_lerp_rgb(default_color,
                              current_color,
                              &anim_color,
                              current_time, opt_anim);

          gamma_backend.rgb(&gamma_backend,
                            video_backend.gamma_ramp_size,
                            anim_color, anim_gamma_value);
#endif

          video_backend.set_gamma(&video_backend, anim_gamma_value);

          time_backend.sleep(&time_backend, opt_delay);
        }

      OL_GAMMA_FREE(anim_gamma_value);
    }

  /* Set gamma */
  video_backend.set_gamma(&video_backend, gamma_value);


  /*** Set color ***/

#ifdef OL_CMAKE_USE_IOS
  color_file = os_backend.open(&os_backend, "color", "wb");
  fwrite(&current_color, 1, sizeof(struct ol_color_s), color_file);
  fclose(color_file);
#endif


  /*** End ***/

 start_end:
 free_default_gamma_value:

  if (gamma_backend.needs_default_gamma || opt_reset)
    OL_GAMMA_FREE(default_gamma_value);

 free_gamma_value:
  OL_GAMMA_FREE(gamma_value);

 free_current_gamma_value:
  OL_GAMMA_FREE(current_gamma_value);

 free_kelvin_backend:
  kelvin_backend.uninit(&kelvin_backend);

 free_time_backend:
  time_backend.uninit(&time_backend);

 free_gamma_backend:
  gamma_backend.uninit(&gamma_backend);

 free_video_backend:
  video_backend.uninit(&video_backend);

 free_os_backend:
  os_backend.uninit(&os_backend);

 end:
  return ret;
}
