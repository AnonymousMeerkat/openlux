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

#ifndef _OL_GAMMA_H
#define _OL_GAMMA_H


#include <stdlib.h>


struct ol_gamma_s
{
  unsigned short* red;
  unsigned short* green;
  unsigned short* blue;
};

#define OL_GAMMA_MALLOC(size, gamma)            \
  {                                             \
    (gamma).red = malloc((size) * 6);           \
    (gamma).green = (gamma).red + (size);       \
    (gamma).blue = (gamma).green + (size);      \
  }

#define OL_GAMMA_FREE(gamma)                    \
  {                                             \
    free((gamma).red);                          \
  }


void
ol_gamma_rgb(unsigned int color, int gamma_ramp_size, unsigned short* red,
             unsigned short* green, unsigned short* blue);

void
ol_gamma_identity(int gamma_ramp_size, unsigned short* red,
                  unsigned short* green, unsigned short* blue);


#endif
