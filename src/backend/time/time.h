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

#ifndef _OPENLUX_BACKEND_TIME_H
#define _OPENLUX_BACKEND_TIME_H


#include <stdint.h>


typedef int64_t ol_time_t;

struct ol_backend_time_s;
struct ol_backend_time_s
{
  void* data;

  int (*init)(struct ol_backend_time_s* self);
  void (*uninit)(struct ol_backend_time_s* self);

  ol_time_t (*get_time)(struct ol_backend_time_s* self);
  void (*sleep)(struct ol_backend_time_s* self, ol_time_t sleep_time);
};


#define OL_BACKEND_TIME_INDEX_POSIX 0
#define OL_BACKEND_TIME_INDEX_MACH 1

int ol_backend_time_init(struct ol_backend_time_s* self, int index, void* data);


#endif
