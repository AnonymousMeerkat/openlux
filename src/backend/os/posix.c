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

#include "os.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "../time/time.h"

#define OL_BACKEND_PREFIX os_posix
#include "../backend.h"


/* Linux standard */
#define OL_BACKEND_OS_POSIX_PATH_MAX 4096

struct ol_backend_os_posix_data_s
{
  char program_home[OL_BACKEND_OS_POSIX_PATH_MAX];
  size_t program_home_len;
};


int
ol_backend_os_posix_init(struct ol_backend_os_s* self)
{
  int ret = 0;
  size_t program_home_pos;

  OL_BACKEND_DATA_INIT(data);

  /* Find the runtime path */

  char* runtime = getenv("XDG_RUNTIME_DIR");
  if (!runtime || runtime[0] == 0)
    {
      runtime = "/tmp";
    }
  program_home_pos = strlen(runtime);

  memcpy(data->program_home, runtime, program_home_pos);
  data->program_home[program_home_pos++] = '/';

  memcpy(data->program_home + program_home_pos, "openlux/", 9);

  data->program_home_len = strlen(data->program_home);


  /* mkdir it */

  if (mkdir(data->program_home, 0755))
    {
      /* FIXME: This doesn't handle the perfectly acceptable ENOTDIR
         case. */
      if (errno != EEXIST)
        {
          ret = -1;
          goto badend;
        }
    }

  self->data = data;
  goto end;
 badend:
  free(data);
 end:
  return ret;
}

void
ol_backend_os_posix_uninit(struct ol_backend_os_s* self)
{
  free(self->data);
}


void
_ol_backend_os_posix_path(struct ol_backend_os_s* self, char* name, char* path)
{
  strcpy(path, OL_BACKEND_DATA()->program_home);
  strcpy(path + OL_BACKEND_DATA()->program_home_len, name);
}

FILE*
ol_backend_os_posix_open(struct ol_backend_os_s* self, char* name, char* mode)
{
  char path[OL_BACKEND_OS_POSIX_PATH_MAX];
  _ol_backend_os_posix_path(self, name, path);

  return fopen(path, mode);
}

int
ol_backend_os_posix_exists(struct ol_backend_os_s* self, char* name)
{
  struct stat file_stat;
  char path[OL_BACKEND_OS_POSIX_PATH_MAX];
  _ol_backend_os_posix_path(self, name, path);

  if (stat(path, &file_stat) < 0)
    return 0;

  if (S_ISDIR(file_stat.st_mode))
    return OL_BACKEND_OS_IS_DIR;
  else
    return OL_BACKEND_OS_IS_FILE;
}


struct ol_backend_os_s ol_backend_os_posix =
  {
    .data = NULL,

#ifdef OL_CMAKE_USE_IOS
    .time_index = OL_BACKEND_TIME_INDEX_MACH,
#else
    .time_index = OL_BACKEND_TIME_INDEX_POSIX,
#endif

    .init   = ol_backend_os_posix_init,
    .uninit = ol_backend_os_posix_uninit,
    .open   = ol_backend_os_posix_open,
    .exists = ol_backend_os_posix_exists
  };
