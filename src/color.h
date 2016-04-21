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

#ifndef _OPENLUX_COLOR_H
#define _OPENLUX_COLOR_H


typedef double ol_color_channel_t;

// TODO: rename to ol_color_s
struct ol_color_t
{
  ol_color_channel_t red, green, blue;
};


#define OL_COLOR_WHITE ((struct ol_color_t) {1., 1., 1.})


#define OL_COLOR_LIMIT(c) (((c) > 1.) ? 1. : (((c) < 0.) ? 0. : (c)))
#define OL_COLOR_BYTE(c) (((int)(c * 255.)) & 0xff)


// emacs indentation :(
#define OL_COLOR_INIT(r, g, b) \
  (struct ol_color_t) {        \
    .red = r,                  \
      .green = g,              \
      .blue = b                \
      }


ol_color_channel_t
ol_color_parse(char* str, ol_color_channel_t old_color);


#endif
