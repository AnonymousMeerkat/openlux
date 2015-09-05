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

#ifndef _OPENLUX_BACKEND_OS_H
#define _OPENLUX_BACKEND_OS_H


#include <stdio.h>

#define OL_BACKEND_OS_IS_FILE 1
#define OL_BACKEND_OS_IS_DIR  2

struct ol_backend_os_s;
struct ol_backend_os_s
{
  void* data;

  int (*init)(struct ol_backend_os_s* self);
  void (*uninit)(struct ol_backend_os_s* self);

  FILE* (*open)(struct ol_backend_os_s* self, char* name, char* mode);
  int (*exists)(struct ol_backend_os_s* self, char* name);
};

int ol_backend_os_init(struct ol_backend_os_s* self);


#endif
