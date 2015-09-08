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

#ifndef _OPENLUX_BACKEND_GAMMA_H
#define _OPENLUX_BACKEND_GAMMA_H


#include "../../color.h"
#include "../../gamma.h"


struct ol_backend_gamma_s;
struct ol_backend_gamma_s
{
  void* data;

  int (*init)(struct ol_backend_gamma_s* self);
  void (*uninit)(struct ol_backend_gamma_s* self);


  void (*rgb)(struct ol_backend_gamma_s* self,
              int gamma_ramp_size,
              ol_color_t color, struct ol_gamma_s gamma);

  void (*identity)(struct ol_backend_gamma_s* self,
              int gamma_ramp_size, struct ol_gamma_s gamma);
};


#define OL_BACKEND_GAMMA_INDEX_ABSOLUTE 0
#define OL_BACKEND_GAMMA_INDEX_RELATIVE 1

int ol_backend_gamma_init(struct ol_backend_gamma_s* self, int index,
                          void* data);


#endif
