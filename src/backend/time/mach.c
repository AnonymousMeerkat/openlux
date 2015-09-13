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

#include "time.h"

#include <mach/mach.h>
#include <mach/mach_time.h>
#include <unistd.h>
#include <stdlib.h>

#define OL_BACKEND_PREFIX time_mach
#include "../backend.h"


struct ol_backend_time_mach_data_s
{
  mach_timebase_info_data_t timebase;
};


int
ol_backend_time_mach_init(struct ol_backend_time_s* self)
{
  OL_BACKEND_DATA_INIT(data);

  mach_timebase_info(&data->timebase);

  self->data = data;
  return 0;
}

void
ol_backend_time_mach_uninit(struct ol_backend_time_s* self)
{
  free(self->data);
}


ol_time_t
ol_backend_time_mach_get_time(struct ol_backend_time_s* self)
{
  uint64_t absolute_time = mach_absolute_time();
  absolute_time *= OL_BACKEND_DATA()->timebase.numer;
  absolute_time /= OL_BACKEND_DATA()->timebase.denom;
  absolute_time /= 1e6;

  return absolute_time;
}

void
ol_backend_time_posix_sleep(struct ol_backend_time_s* self,
                            ol_time_t sleep_time);


struct ol_backend_time_s ol_backend_time_mach =
  {
    .data     = NULL,

    .init     = ol_backend_time_mach_init,
    .uninit   = ol_backend_time_mach_uninit,

    .get_time = ol_backend_time_mach_get_time,
    .sleep    = ol_backend_time_posix_sleep
  };
