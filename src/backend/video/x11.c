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

#include "video.h"
#include "../../log.h"

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

#define OL_BACKEND_PREFIX video_x11
#include "../backend.h"

#include "../gamma/gamma.h"


struct ol_backend_video_x11_data_s
{
  Display* display;
  int screen;
};


int
ol_backend_video_x11_init(struct ol_backend_video_s* self)
{
  int ret = 0;

  int vidmode_maj, vidmode_min;
  int extension_event, extension_error;

  OL_BACKEND_DATA_INIT(data);
  int gamma_ramp_size;


  data->display = XOpenDisplay(NULL);

  if (!data->display)
    {
      OL_LOG_ERR("Unable to connect to X11 display");

      ret = 1;
      goto badend;
    }


  /* Check if XF86VidMode extension is supported
     TODO: Check version */
  if (!XF86VidModeQueryVersion(data->display, &vidmode_maj, &vidmode_min) ||
      !XF86VidModeQueryExtension(data->display, &extension_event, &extension_error))
    {
      OL_LOG_ERR("XF86VidMode extension not supported");

      ret = -1;
      goto badend;
    }


  data->screen = DefaultScreen(data->display);

  gamma_ramp_size = -1;
  XF86VidModeGetGammaRampSize(data->display, data->screen, &gamma_ramp_size);
  if (gamma_ramp_size != 02000 && gamma_ramp_size != 0400)
    {
      OL_LOG_ERR("Only 24-bit or 32-bit depths are supported at this time");

      ret = -1;
      goto badend;
    }

  if (gamma_ramp_size == -1)
    {
      OL_LOG_ERR("Unable to find gamma ramp size");

      ret = -1;
      goto badend;
    }


  self->data = data;
  self->gamma_ramp_size = gamma_ramp_size;

  goto end;
 badend:
  free(data);
 end:
  return ret;
}

void
ol_backend_video_x11_uninit(struct ol_backend_video_s* self)
{
  XCloseDisplay(OL_BACKEND_DATA()->display);
  free(self->data);
}


void
ol_backend_video_x11_get_gamma(struct ol_backend_video_s* self,
                          struct ol_gamma_s gamma)
{
  XF86VidModeGetGammaRamp(OL_BACKEND_DATA()->display, OL_BACKEND_DATA()->screen,
                          self->gamma_ramp_size,
                          gamma.red, gamma.green, gamma.blue);
}

void
ol_backend_video_x11_set_gamma(struct ol_backend_video_s* self,
                          struct ol_gamma_s gamma)
{
  XF86VidModeSetGammaRamp(OL_BACKEND_DATA()->display, OL_BACKEND_DATA()->screen,
                          self->gamma_ramp_size,
                          gamma.red, gamma.green, gamma.blue);
}

struct ol_backend_video_s ol_backend_video_x11 =
{
  .data = NULL,
  .gamma_ramp_size = 0,

  .gamma_index = OL_BACKEND_GAMMA_INDEX_ABSOLUTE,
  .gamma_data = NULL,

  .init = ol_backend_video_x11_init,
  .uninit = ol_backend_video_x11_uninit,
  .get_gamma = ol_backend_video_x11_get_gamma,
  .set_gamma = ol_backend_video_x11_set_gamma
};
