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

#ifndef _OL_GAMMA_H
#define _OL_GAMMA_H


#include <string.h>
#include <stdlib.h>


#ifdef OL_CMAKE_USE_IOS
typedef uint32_t ol_gamma_t;
#else
typedef unsigned short ol_gamma_t;
#endif

struct ol_gamma_s
{
  ol_gamma_t* red;
  ol_gamma_t* green;
  ol_gamma_t* blue;
};

#define OL_GAMMA_ELEMENTS(size) ((size) * 3)
#define OL_GAMMA_SIZE(size) (OL_GAMMA_ELEMENTS(size) * sizeof(ol_gamma_t))

#define _OL_GAMMA_INIT(size, gamma)             \
  {                                             \
    (gamma).green = (gamma).red + (size);       \
    (gamma).blue = (gamma).green + (size);      \
  }

#define OL_GAMMA_MALLOC(size, gamma)                            \
  {                                                             \
    (gamma).red = malloc(OL_GAMMA_SIZE(size));                  \
    if (!(gamma).red)                                           \
      {                                                         \
        OL_LOG_ERR("Unable to allocate %ld bytes for "#gamma,   \
                   OL_GAMMA_SIZE(size));                        \
      }                                                         \
    _OL_GAMMA_INIT(size, gamma);                                \
  }

#define OL_GAMMA_FREE(gamma)                    \
  {                                             \
    if ((gamma).red)                            \
      {                                         \
        free((gamma).red);                      \
        (gamma).red = NULL;                     \
      }                                         \
  }

#define OL_GAMMA_COPY(dest, src, size)                  \
  {                                                     \
    memcpy((dest).red, (src).red, OL_GAMMA_SIZE(size)); \
    _OL_GAMMA_INIT(size, dest);                         \
  }


#endif
