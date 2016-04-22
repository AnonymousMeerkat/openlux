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

#include "kelvin.h"

#include "../../util.h"
#include <math.h>
#include <stddef.h>

int
ol_backend_kelvin_flux_init(struct ol_backend_kelvin_s* self)
{
  OL_UTIL_UNUSED(self);
  return 0;
}

void
ol_backend_kelvin_flux_uninit(struct ol_backend_kelvin_s* self)
{
  OL_UTIL_UNUSED(self);
}


/*** F.lux's windows algorithm, as of 21 april 2016 ***/

double
_ol_flux_curve(double curvei, double kelvin)
{
  return
    pow(curvei * 0.000000001, -5.0) *
    3.74183e-16 / (exp(0.014388 / (curvei * 0.000000001 * kelvin)) - 1.0);
}

extern float* ol_backend_kelvin_flux_data;

void
_ol_flux_kelvinkore(ol_kelvin_t kelvin,
                    ol_color_channel_t* red,
                    ol_color_channel_t* green,
                    ol_color_channel_t* blue)
{
  int curvei;
  float *datap = ol_backend_kelvin_flux_data;
  double curve;
  ol_color_channel_t temp;

  *red = 0.0;
  *green = 0.0;
  *blue = 0.0;

  for (curvei = 360; curvei <= 830; curvei += 5)
    {
      curve = _ol_flux_curve((double)curvei, kelvin);

      temp = datap[-1] * curve; // FIXME: is this really supposed to be before?
      datap += 3;

      *red   += 5.0 * temp;
      *green += 5.0 * (curve * datap[-3]);
      *blue  += 5.0 * (curve * datap[-2]);
    }
}

void
_ol_flux_normkk(ol_kelvin_t kelvin,
                ol_color_channel_t* red,
                ol_color_channel_t* green,
                ol_color_channel_t* blue)
{
  ol_color_channel_t total;

  _ol_flux_kelvinkore(kelvin, red, green, blue);

  total = *red + *green + *blue;

  *red   /= total;
  *green /= total;
  *blue  /= total;
}

ol_color_channel_t
_ol_flux_processchannel(ol_color_channel_t channel)
{
  ol_color_channel_t temp;

  if (channel > 0.0031308)
    {
      temp = pow(channel, 0.4166666567325592);
      return (temp * 1.054999947547913) - 0.05499999970197678;
    }
  else
    {
      return channel * 12.92000007629395;
    }
}

void
_ol_flux_normrgb(ol_color_channel_t* red,
                 ol_color_channel_t* green,
                 ol_color_channel_t* blue)
{
  ol_color_channel_t maxchannel = *red;

  if (maxchannel < *green)
    maxchannel = *green;

  if (maxchannel < *blue)
    maxchannel = *blue;


  if (maxchannel != 0.0)
    {
      *red   /= maxchannel;
      *green /= maxchannel;
      *blue  /= maxchannel;
    }


  if (*red < 0.0)
    *red = 0.0;

  if (*green < 0.0)
    *green = 0.0;

  if (*blue < 0.0)
    *blue = 0.0;

}

void
_ol_flux_processrgb(ol_color_channel_t ired,
                    ol_color_channel_t igreen,
                    ol_color_channel_t iblue,
                    ol_color_channel_t* ored,
                    ol_color_channel_t* ogreen,
                    ol_color_channel_t* oblue)
{
  *ored   = (ired * 3.2406)   - (igreen * 1.5372) - (iblue * 0.4986);
  *ogreen = (igreen * 1.8758) - (ired * 0.9689)   + (iblue * 0.0415);
  *oblue  = (ired * 0.0557)   - (igreen * 0.204)  + (iblue * 1.057);

  *ored   = _ol_flux_processchannel(*ored);
  *ogreen = _ol_flux_processchannel(*ogreen);
  *oblue  = _ol_flux_processchannel(*oblue);

  _ol_flux_normrgb(ored, ogreen, oblue);
}

void
_ol_flux_kelvin_raw(ol_kelvin_t kelvin,
                    ol_color_channel_t* ored,
                    ol_color_channel_t* ogreen,
                    ol_color_channel_t* oblue)
{
  ol_color_channel_t red;
  ol_color_channel_t green;
  ol_color_channel_t blue;

  _ol_flux_normkk(kelvin, &red, &green, &blue);

  if (red != 0.0 && green != 0.0 && blue != 0.0)
    {
      _ol_flux_processrgb( red,  green,  blue,
                           ored, ogreen, oblue);
    }
}

ol_color_channel_t _ol_flux_whitekred   = 0.0;
ol_color_channel_t _ol_flux_whitekgreen = 0.0;
ol_color_channel_t _ol_flux_whitekblue  = 0.0;

void
_ol_flux_kelvin(ol_kelvin_t kelvin,
                ol_color_channel_t* red,
                ol_color_channel_t* green,
                ol_color_channel_t* blue)
{
  ol_color_channel_t temp;

  if (_ol_flux_whitekred == 0.0)
    {
      _ol_flux_kelvin_raw(6500.0,
                          &_ol_flux_whitekred,
                          &_ol_flux_whitekgreen,
                          &_ol_flux_whitekblue);

      _ol_flux_whitekred   = 1.0 / _ol_flux_whitekred;
      _ol_flux_whitekgreen = 1.0 / _ol_flux_whitekgreen;
      _ol_flux_whitekblue  = 1.0 / _ol_flux_whitekblue;
    }

  _ol_flux_kelvin_raw(kelvin, red, green, blue);

  if (kelvin > 5500.0 && kelvin <= 6500.0)
    {
      temp = (kelvin - 5500.0) / 1000.0;

      *red   *= (_ol_flux_whitekred   - 1.0) * temp + 1.0;
      *green *= (_ol_flux_whitekgreen - 1.0) * temp + 1.0;
      *blue  *= (_ol_flux_whitekblue  - 1.0) * temp + 1.0;
    }
  else if (kelvin > 6500.0 && kelvin < 7500.0)
    {
      temp = (kelvin - 6500.0) / 1000.;

      *red   *= (_ol_flux_whitekred   - 1.0) * temp + 1.0;
      *green *= (_ol_flux_whitekgreen - 1.0) * temp + 1.0;
      *blue  *= (_ol_flux_whitekblue  - 1.0) * temp + 1.0;
    }
}

struct ol_color_s
ol_backend_kelvin_flux_kelvin(struct ol_backend_kelvin_s* self,
                              ol_kelvin_t kelvin)
{
  OL_UTIL_UNUSED(self);

  ol_color_channel_t red   = 0.;
  ol_color_channel_t green = 0.;
  ol_color_channel_t blue  = 0.;

  _ol_flux_kelvin(kelvin, &red, &green, &blue);

  red   = OL_COLOR_LIMIT(red);
  green = OL_COLOR_LIMIT(green);
  blue  = OL_COLOR_LIMIT(blue);

  return OL_COLOR_INIT(red, green, blue);
}


struct ol_backend_kelvin_s ol_backend_kelvin_flux =
{
  .data     = NULL,

  .init     = ol_backend_kelvin_flux_init,
  .uninit   = ol_backend_kelvin_flux_uninit,

  .kelvin   = ol_backend_kelvin_flux_kelvin
};
