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

#ifndef _OPENLUX_BACKEND_VIDEO_H
#define _OPENLUX_BACKEND_VIDEO_H


#include "../../gamma.h"

struct ol_backend_video_s;
struct ol_backend_video_s
{
  void* data;
  int gamma_ramp_size;

  int (*init)(struct ol_backend_video_s* self);
  void (*uninit)(struct ol_backend_video_s* self);
  void (*get_gamma)(struct ol_backend_video_s* self,
                    struct ol_gamma_s gamma);
  void (*set_gamma)(struct ol_backend_video_s* self,
                    struct ol_gamma_s gamma);
};

int
ol_backend_video_init(struct ol_backend_video_s* self);


#endif
