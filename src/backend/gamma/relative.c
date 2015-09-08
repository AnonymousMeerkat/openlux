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

#include "gamma.h"

#include "../../util.h"
#include "../../log.h"

#define OL_BACKEND_PREFIX gamma_relative
#include "../backend.h"


/* hack for OL_BACKEND_DATA() */
#define ol_backend_gamma_relative_data_s ol_gamma_s

int
ol_backend_gamma_relative_init(struct ol_backend_gamma_s* self)
{
  OL_UTIL_UNUSED(self);
  return 0;
}

void
ol_backend_gamma_relative_uninit(struct ol_backend_gamma_s* self)
{
  OL_UTIL_UNUSED(self);
}


/* FIXME: Doesn't use gamma_ramp_size */
void
ol_backend_gamma_relative_rgb(struct ol_backend_gamma_s* self,
                              int gamma_ramp_size,
                              ol_color_t color, struct ol_gamma_s gamma)
{
  unsigned int redc   = 0;
  unsigned int greenc = 0;
  unsigned int bluec  = 0;

  unsigned int color_div;
  unsigned int color_mod;

  for (int j = 0; j < 256; j++)
    {
#define do_color(col)                                                   \
      {                                                                 \
        color_div = col##c >> 8;                                        \
        color_mod = col##c & 0xff;                                      \
        gamma.col[j] =                                                  \
          OL_BACKEND_DATA()->col[color_div] +                           \
          (((OL_BACKEND_DATA()->col[color_div + 1] -                    \
             OL_BACKEND_DATA()->col[color_div])                         \
            * color_mod)                                                \
           >> 8);                                                       \
      }

      do_color(red);
      do_color(green);
      do_color(blue);

      redc += OL_COLOR_RED(color);
      greenc += OL_COLOR_GREEN(color);
      bluec += OL_COLOR_BLUE(color);
    }
}

void
ol_backend_gamma_relative_identity(struct ol_backend_gamma_s* self,
                                   int gamma_ramp_size, struct ol_gamma_s gamma)
{
  OL_UTIL_UNUSED(self);
  OL_UTIL_UNUSED(gamma_ramp_size);
  OL_UTIL_UNUSED(gamma);

  OL_LOG_ERR("Identity not supported for relative gamma");
}


struct ol_backend_gamma_s ol_backend_gamma_relative =
  {
    .data = NULL,

    .init     = ol_backend_gamma_relative_init,
    .uninit   = ol_backend_gamma_relative_uninit,
    .rgb      = ol_backend_gamma_relative_rgb,
    .identity = ol_backend_gamma_relative_identity
  };
