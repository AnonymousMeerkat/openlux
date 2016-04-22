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

#include "kelvin.h"

#include "../../util.h"
#include <math.h>
#include <stddef.h>

int
ol_backend_kelvin_neilb_init(struct ol_backend_kelvin_s* self)
{
  OL_UTIL_UNUSED(self);
  return 0;
}

void
ol_backend_kelvin_neilb_uninit(struct ol_backend_kelvin_s* self)
{
  OL_UTIL_UNUSED(self);
}


/* Implementation of
   http://www.zombieprototypes.com/?p=210 */
struct ol_color_s
ol_backend_kelvin_neilb_kelvin(struct ol_backend_kelvin_s* self,
                               ol_kelvin_t kelvin)
{
  OL_UTIL_UNUSED(self);

  ol_color_channel_t red   = 0.;
  ol_color_channel_t green = 0.;
  ol_color_channel_t blue  = 0.;

  kelvin /= 100.;

  if (kelvin < 66.)
    {
      red = 255.;

      green = kelvin - 2.;
      green = -155.25485562709179 - 0.44596950469579133 * green + 104.49216199393888 * log(green);
    }
  else
    {
      red = kelvin - 55.0;
      red = 351.97690566805693 + 0.114206453784165 * red - 40.25366309332127 * log(red);

      green = kelvin - 50.0;
      green = 325.4494125711974 + 0.07943456536662342 * green - 28.0852963507957 * log(green);
    }

  if (kelvin >= 66.)
    {
      blue = 255.;
    }
  else
    {
      if (kelvin <= 20.)
        {
          blue = 0.;
        }
      else
        {
          blue = kelvin - 10;
          blue = -254.76935184120902 + 0.8274096064007395 * blue + 115.67994401066147 * log(blue);
        }
    }

  red   /= 255.;
  green /= 255.;
  blue  /= 255.;

  red   = OL_COLOR_LIMIT(red);
  green = OL_COLOR_LIMIT(green);
  blue  = OL_COLOR_LIMIT(blue);

  return OL_COLOR_INIT(red, green, blue);
}


struct ol_backend_kelvin_s ol_backend_kelvin_neilb =
{
  .data     = NULL,

  .init     = ol_backend_kelvin_neilb_init,
  .uninit   = ol_backend_kelvin_neilb_uninit,

  .kelvin   = ol_backend_kelvin_neilb_kelvin
};
