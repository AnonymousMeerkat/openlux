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

#ifndef _OPENLUX_BACKEND_H
#define _OPENLUX_BACKEND_H


struct ol_backend_s;
struct ol_backend_s
{
  void* data;
  int gamma_ramp_size;

  int (*init)(struct ol_backend_s* backend);
  void (*uninit)(struct ol_backend_s* backend);
  void (*get_gamma)(struct ol_backend_s* backend, unsigned short* red,
                    unsigned short* green, unsigned short* blue);
  void (*set_gamma)(struct ol_backend_s* backend, unsigned short* red,
                    unsigned short* green, unsigned short* blue);
};

int ol_backend_init(struct ol_backend_s* backend);


#endif
