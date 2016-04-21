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

#include "gamma.h"

#include "../../util.h"
#include "../../log.h"

#include <string.h>
#include <stdlib.h>

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
  if (self->data)
    free(self->data);
}


void
ol_backend_gamma_relative_set_default_gamma(struct ol_backend_gamma_s* self,
                                            struct ol_gamma_s gamma)
{
  self->data = malloc(sizeof(struct ol_gamma_s));
  memcpy(self->data, &gamma, sizeof(struct ol_gamma_s));
}

void
ol_backend_gamma_relative_rgb(struct ol_backend_gamma_s* self,
                              int gamma_ramp_size,
                              struct ol_color_s color,
                              struct ol_gamma_s gamma)
{
  ol_color_channel_t redc   = 0;
  ol_color_channel_t greenc = 0;
  ol_color_channel_t bluec  = 0;

  unsigned int color_div;

  for (int j = 0; j < gamma_ramp_size; j++)
    {
#define do_color(col)                                           \
      {                                                         \
        color_div = ((int)col##c);                              \
        gamma.col[j] = OL_BACKEND_DATA()->col[color_div];       \
      }

      do_color(red);
      do_color(green);
      do_color(blue);

      redc   += color.red;
      greenc += color.green;
      bluec  += color.blue;
    }
}

void
ol_backend_gamma_relative_identity(struct ol_backend_gamma_s* self,
                                   int gamma_ramp_size, struct ol_gamma_s gamma)
{
  self->rgb(self, gamma_ramp_size, OL_COLOR_WHITE, gamma);
}


struct ol_backend_gamma_s ol_backend_gamma_relative =
{
  .data                = NULL,
  .needs_default_gamma = 1,

  .init                = ol_backend_gamma_relative_init,
  .uninit              = ol_backend_gamma_relative_uninit,

  .set_default_gamma   = ol_backend_gamma_relative_set_default_gamma,
  .rgb                 = ol_backend_gamma_relative_rgb,
  .identity            = ol_backend_gamma_relative_identity
};
