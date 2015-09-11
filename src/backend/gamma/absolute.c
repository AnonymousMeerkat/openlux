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

#define OL_BACKEND_PREFIX gamma_absolute
#include "../backend.h"



int
ol_backend_gamma_absolute_init(struct ol_backend_gamma_s* self)
{
  OL_UTIL_UNUSED(self);
  return 0;
}

void
ol_backend_gamma_absolute_uninit(struct ol_backend_gamma_s* self)
{
  OL_UTIL_UNUSED(self);
}


void
ol_backend_gamma_absolute_set_default_gamma(struct ol_backend_gamma_s* self,
                                            struct ol_gamma_s gamma)
{
  OL_UTIL_UNUSED(self);
  OL_UTIL_UNUSED(gamma);
}

void
ol_backend_gamma_absolute_rgb(struct ol_backend_gamma_s* self,
                              int gamma_ramp_size,
                              ol_color_t color, struct ol_gamma_s gamma)
{
  /* Calculated */
  ol_gamma_t cred   = 0;
  ol_gamma_t cgreen = 0;
  ol_gamma_t cblue  = 0;

  /* Current */
  unsigned int redc   = 0;
  unsigned int greenc = 0;
  unsigned int bluec  = 0;

  /* Increment */
  unsigned int redi   = 0xffff * OL_COLOR_RED(color);
  unsigned int greeni = 0xffff * OL_COLOR_GREEN(color);
  unsigned int bluei  = 0xffff * OL_COLOR_BLUE(color);

  unsigned int range = 0xff * (gamma_ramp_size - 1);

  OL_UTIL_UNUSED(self);

  for (int i = 0; i < gamma_ramp_size; i++)
    {
      cred = redc / range;
      redc += redi;
      gamma.red[i] = cred;

      cgreen = greenc / range;
      greenc += greeni;
      gamma.green[i] = cgreen;

      cblue = bluec / range;
      bluec += bluei;
      gamma.blue[i] = cblue;
    }
}

void
ol_backend_gamma_absolute_identity(struct ol_backend_gamma_s* self,
                                   int gamma_ramp_size, struct ol_gamma_s gamma)
{
  OL_UTIL_UNUSED(self);

  for (int i = 0; i < gamma_ramp_size; i++)
    {
      gamma.red[i] = i << 8;
      gamma.green[i] = i << 8;
      gamma.blue[i] = i << 8;
    }
}


struct ol_backend_gamma_s ol_backend_gamma_absolute =
  {
    .data                = NULL,
    .needs_default_gamma = 0,

    .init                = ol_backend_gamma_absolute_init,
    .uninit              = ol_backend_gamma_absolute_uninit,

    .set_default_gamma   = ol_backend_gamma_absolute_set_default_gamma,
    .rgb                 = ol_backend_gamma_absolute_rgb,
    .identity            = ol_backend_gamma_absolute_identity
  };
