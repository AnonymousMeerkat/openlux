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

#include "kelvin.h"
#include "color.h"
#include <math.h>

/* Implementation of
   http://www.tannerhelland.com/4435/convert-temperature-rgb-algorithm-code/ */
ol_color_t ol_kelvin_rgb(unsigned int kelvin) {
  float red   = 0;
  float green = 0;
  float blue  = 0;

  kelvin /= 100;

  if (kelvin <= 66)
    {
      red = 255;

      green = 99.4708025861 * log(kelvin) - 161.1195681661;
      green = OL_COLOR_LIMIT(green);
    }
  else
    {
      red = 329.698727446 * pow((kelvin - 60), -0.1332047592);
      red = OL_COLOR_LIMIT(red);

      green = 288.1221695283 * pow((kelvin - 60), -0.0755148492);
      green = OL_COLOR_LIMIT(green);
    }

  if (kelvin >= 66)
    {
      blue = 255;
    }
  else
    {
      if (kelvin <= 19)
        {
          blue = 0;
        }
      else
        {
          blue = 138.5177312231 * log(kelvin - 10) - 305.0447927307;
          blue = OL_COLOR_LIMIT(blue);
        }
    }

  ol_color_byte_t redc = red;
  ol_color_byte_t greenc = green;
  ol_color_byte_t bluec = blue;

  return OL_COLOR_INIT(redc, greenc, bluec);
}
