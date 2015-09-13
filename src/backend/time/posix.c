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

#include "time.h"

#include <unistd.h>
#include <time.h>
#include "../../util.h"

int
ol_backend_time_posix_init(struct ol_backend_time_s* self)
{
  OL_UTIL_UNUSED(self);
  return 0;
}

void
ol_backend_time_posix_uninit(struct ol_backend_time_s* self)
{
  OL_UTIL_UNUSED(self);
}


ol_time_t
ol_backend_time_posix_get_time(struct ol_backend_time_s* self)
{
  OL_UTIL_UNUSED(self);

#if _POSIX_TIMERS > 0 && defined(_POSIX_MONOTONIC_CLOCK)

  struct timespec tp;

#ifdef CLOCK_MONOTONIC_RAW
  clock_gettime(CLOCK_MONOTONIC_RAW, &tp);
#else
  clock_gettime(CLOCK_MONOTONIC, &tp);
#endif /* CLOCK_MONOTONIC_RAW */

  return (tp.tv_sec * 1000) + (tp.tv_nsec / 1000000);

#else /* _POSIX_TIMERS > 0 && _POSIX_MONOTONIC_CLOCK */

  return -1;

#endif
}

void
ol_backend_time_posix_sleep(struct ol_backend_time_s* self,
                            ol_time_t sleep_time)
{
  OL_UTIL_UNUSED(self);

#if                                             \
  defined(OL_CMAKE_HAVE_CLOCK_NANOSLEEP) ||     \
  defined(OL_CMAKE_HAVE_NANOSLEEP)

  struct timespec tp;
  tp.tv_sec = sleep_time / 1000;
  tp.tv_nsec = (sleep_time * 1000000) % 1000000000;

#endif

#ifdef OL_CMAKE_HAVE_CLOCK_NANOSLEEP
  clock_nanosleep(CLOCK_MONOTONIC, 0, &tp, NULL);
#elif defined(OL_CMAKE_HAVE_NANOSLEEP)
  nanosleep(&tp, NULL);
#else
  usleep(sleep_time * 1000);
#endif
}


struct ol_backend_time_s ol_backend_time_posix =
  {
    .data     = NULL,

    .init     = ol_backend_time_posix_init,
    .uninit   = ol_backend_time_posix_uninit,

    .get_time = ol_backend_time_posix_get_time,
    .sleep    = ol_backend_time_posix_sleep
  };
