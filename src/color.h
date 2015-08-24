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


typedef unsigned int ol_color_t;
typedef unsigned char ol_color_byte_t;


#define OL_COLOR_LIMIT(c) (((c) > 255) ? 255 : (((c) < 0) ? 0 : (c)))
#define OL_COLOR_BYTE(c) ((c) & 0xff)


#define OL_COLOR_INIT(r, g, b)                  \
  (((OL_COLOR_BYTE(r) << 16) |                  \
    (OL_COLOR_BYTE(g) << 8)) |                  \
   OL_COLOR_BYTE(b))


#define OL_COLOR_RED(c) (((c) >> 16) & 0xff)
#define OL_COLOR_GREEN(c) (((c) >> 8) & 0xff)
#define OL_COLOR_BLUE(c) ((c) & 0xff)

#define OL_COLOR_RED_GREEN(c) ((((c) >> 8) & 0xffff) << 8)
#define OL_COLOR_RED_BLUE(c) ((OL_COLOR_RED(c) << 16) | (OL_COLOR_BLUE(c)))
#define OL_COLOR_GREEN_BLUE(c) ((c) & 0xffff)


#define OL_COLOR_SET_RED(c, r) ((OL_COLOR_BYTE(r) << 16) | (OL_COLOR_GREEN_BLUE(c)))
#define OL_COLOR_SET_GREEN(c, g) (((OL_COLOR_BYTE(g) << 8) | (OL_COLOR_RED_BLUE(c))))
#define OL_COLOR_SET_BLUE(c, b) ((OL_COLOR_BYTE(b)) | (OL_COLOR_RED_GREEN(c)))


#endif
