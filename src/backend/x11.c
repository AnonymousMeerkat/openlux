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

#include "x11.h"
#include "../log.h"

#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>

struct _ol_backend_x11_data
{
  Display* display;
  int screen;
};
#define _OL_DATA(backend) ((struct _ol_backend_x11_data*)((backend)->data))

static int
_ol_backend_x11_init(struct ol_backend_s* backend)
{
  int ret = 0;

  int vidmode_maj, vidmode_min;
  int extension_event, extension_error;

  struct _ol_backend_x11_data* data =
    malloc(sizeof(struct _ol_backend_x11_data));
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


  backend->data = data;
  backend->gamma_ramp_size = gamma_ramp_size;

  goto end;
 badend:
  free(data);
 end:
  return ret;
}

static void
_ol_backend_x11_uninit(struct ol_backend_s* backend)
{
  XCloseDisplay(_OL_DATA(backend)->display);
  free(backend->data);
}


static void
_ol_backend_x11_get_gamma(struct ol_backend_s* backend, unsigned short* red,
                               unsigned short* green, unsigned short* blue)
{
  XF86VidModeGetGammaRamp(_OL_DATA(backend)->display, _OL_DATA(backend)->screen,
                          backend->gamma_ramp_size, red, green, blue);
}

static void
_ol_backend_x11_set_gamma(struct ol_backend_s* backend, unsigned short* red,
                               unsigned short* green, unsigned short* blue)
{
  XF86VidModeSetGammaRamp(_OL_DATA(backend)->display, _OL_DATA(backend)->screen,
                          backend->gamma_ramp_size, red, green, blue);
}

struct ol_backend_s ol_backend_x11 =
{
  .data = NULL,
  .gamma_ramp_size = 0,

  .init = _ol_backend_x11_init,
  .uninit = _ol_backend_x11_uninit,
  .get_gamma = _ol_backend_x11_get_gamma,
  .set_gamma = _ol_backend_x11_set_gamma
};
