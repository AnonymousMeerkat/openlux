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

ol_color_byte_t
ol_color_parse(char* str, ol_color_byte_t old_color)
{
  if (!strcmp(str, "auto"))
    {
      return old_color;
    }

  bool change = false;
  if (str[0] == '+' || str[0] == '-')
    {
      str++;
      change = true;
    }

  /* FIXME: atoi with non-numeric inputs causes undefined behaviour */
  int c = atoi(str);
  c = OL_COLOR_LIMIT(c);

  if (change)
    {
      c = (0x80000000 | (c)) ^ (0x80000000 - !!(str[-1] - 43));
      if (c & 0x80000000) c++;
      c += old_color;
      return OL_COLOR_LIMIT(c);
    }
  return c;
}
