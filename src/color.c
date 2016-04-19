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

#include "color.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>


ol_color_channel_t
ol_color_parse(char* str, ol_color_channel_t old_color)
{
  if (!strcmp(str, "auto"))
    return old_color;


  bool change = false;
  if (str[0] == '+' || str[0] == '-')
    {
      str++;
      change = true;
    }


  /* FIXME: atoi with non-numeric inputs causes undefined behaviour */
  ol_color_channel_t c;

  if (strchr(str, '.'))
    c = atof(str);
  else
    c = ((ol_color_channel_t)atoi(str)) / 255.;

  c = OL_COLOR_LIMIT(c);

  if (change)
    {
      if (str[-1] == '+')
        return OL_COLOR_LIMIT(old_color + c);
      else
        return OL_COLOR_LIMIT(old_color - c);
    }

  return c;
}
