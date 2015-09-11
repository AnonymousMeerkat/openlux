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

/* Largely based on iomfsetgamma from saurik's uikittools */

#include "video.h"

#include <mach/mach.h>
#include <IOKit/IOKitLib.h>
#include <dlfcn.h>

kern_return_t IOMobileFramebufferOpen(io_service_t, mach_port_t, void*, void*);
kern_return_t IOMobileFramebufferSetGammaTable(void*, void*);


#define OL_BACKEND_PREFIX video_ios
#include "../backend.h"

#include "../gamma/gamma.h"
#include "../../log.h"


struct ol_backend_video_ios_data_s
{
  mach_port_t port;
  io_service_t service;
  void* framebuffer;

  kern_return_t (*_get_gamma)(void*, void*);
};


int
ol_backend_video_ios_init(struct ol_backend_video_s* self)
{
  int ret = 0;

  kern_return_t kerror;

  OL_BACKEND_DATA_INIT(data);

  data->port = mach_task_self();

  /* Find LCD service */
#define _OL_FIND_SERVICE(name)                                          \
  IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching(name))
  ; /* emacs */

  data->service = _OL_FIND_SERVICE("AppleCLCD");
  if (!data->service)
    {
      data->service = _OL_FIND_SERVICE("AppleH1CLCD");
    }
  if (!data->service)
    {
      data->service = _OL_FIND_SERVICE("AppleH2CLCD");
    }

  if (!data->service)
    {
      OL_LOG_ERR("Unable to find LCD service");

      ret = -1;
      goto badend;
    }

#undef _OL_FIND_SERVICE

  kerror = IOMobileFramebufferOpen(data->service, data->port,
                                   0, &data->framebuffer);
  if (kerror)
    {
      OL_LOG_ERR("Unable to open framebuffer");

      ret = -1;
      goto badend;
    }

  data->_get_gamma = dlsym(RTLD_DEFAULT, "IOMobileFramebufferGetGammaTable");
  if (!data->_get_gamma)
    {
      OL_LOG_ERR("Unable to load IOMobileFramebufferGetGammaTable, "
                 "are you on iOS <4?");

      ret = -1;
      goto badend;
    }

  self->data = data;

  goto end;
 badend:
  free(data);
 end:
  return 0;
}

void
ol_backend_video_ios_uninit(struct ol_backend_video_s* self)
{
  /* XXX: Nothing else needs to be done? */
  free(self->data);
}


void
ol_backend_video_ios_get_gamma(struct ol_backend_video_s* self,
                               struct ol_gamma_s gamma)
{
  /* XXX: Ignoring error */
  OL_BACKEND_DATA()->_get_gamma(OL_BACKEND_DATA()->framebuffer,
                                gamma.red);
}


void
ol_backend_video_ios_set_gamma(struct ol_backend_video_s* self,
                               struct ol_gamma_s gamma)
{
  /* XXX: Ignoring error */
  IOMobileFramebufferSetGammaTable(OL_BACKEND_DATA()->framebuffer,
                                   gamma.red);
}


struct ol_backend_video_s ol_backend_video_ios =
  {
    .data            = NULL,
    .gamma_ramp_size = 256,

    .gamma_index     = OL_BACKEND_GAMMA_INDEX_RELATIVE,
    .gamma_data      = NULL,

    .init            = ol_backend_video_ios_init,
    .uninit          = ol_backend_video_ios_uninit,
    .get_gamma       = ol_backend_video_ios_get_gamma,
    .set_gamma       = ol_backend_video_ios_set_gamma
  };
