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

#define color_red(c) (((c) >> 16) & 0xff)
#define color_green(c) (((c) >> 8) & 0xff)
#define color_blue(c) ((c) & 0xff)

#define color_init(r, g, b) ((((r) << 16) | ((g) << 8)) | b)

#define color_set_red(c, r) (((r) << 16) | ((c) & 0xffff))
#define color_set_green(c, g) ((((g) << 8) | (color_red(c) << 16)) | (color_blue(c)))
#define color_set_blue(c, b) ((b) | ((((c) >> 8) & 0xffff) << 8))

#define color_limit(c) (((c) > 255) ? 255 : (((c) < 0) ? 0 : (c)))
