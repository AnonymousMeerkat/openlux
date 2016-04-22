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

#ifndef _OPENLUX_BACKEND_KELVIN_H
#define _OPENLUX_BACKEND_KELVIN_H


#include "../../color.h"


struct ol_backend_kelvin_s;
struct ol_backend_kelvin_s
{
  void* data;

  int (*init)(struct ol_backend_kelvin_s* self);
  void (*uninit)(struct ol_backend_kelvin_s* self);

  struct ol_color_s (*kelvin)(struct ol_backend_kelvin_s* self,
                              double kelvin);
};


#define OL_BACKEND_KELVIN_INDEX_NEILB  0
#define OL_BACKEND_KELVIN_INDEX_TANNER 1
#define OL_BACKEND_KELVIN_INDEX_FLUX   2

int ol_backend_kelvin_init(struct ol_backend_kelvin_s* self, int index, void* data);


#endif
