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

#include "animate.h"
#include <time.h>
#include <unistd.h>


ol_time_t
ol_animate_gettime(ol_time_t start_time)
{
  struct timespec tp;

#ifdef CLOCK_MONOTONIC_RAW
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
#else
  clock_gettime(CLOCK_MONOTONIC, &tp);
#endif

  return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000) - start_time;
}

void
ol_animate_sleep(ol_time_t time)
{
  usleep(time * 1000);
}

void
ol_animate_lerp(struct ol_gamma_s current,
                struct ol_gamma_s new,
                struct ol_gamma_s anim,
                int gamma_ramp_size,
                ol_time_t time,
                ol_time_t end_time)
{
  for (int i = 0; i < gamma_ramp_size; i++)
    {
#define _lerp(start, end)                                               \
      (((ol_time_t)(start)) +                                           \
       (((((ol_time_t)(end)) - ((ol_time_t)(start))) * time) / end_time))

      anim.red[i]   = _lerp(current.red[i]  , new.red[i]  );
      anim.green[i] = _lerp(current.green[i], new.green[i]);
      anim.blue[i]  = _lerp(current.blue[i] , new.blue[i] );
    }
}
