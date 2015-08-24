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

#ifndef _OPENLUX_LOG_H
#define _OPENLUX_LOG_H


#include <stdio.h>

#define _OL_LOG_TEMPLATE(head, stream, ...)     \
  {                                             \
    fputs("[" head "] ", stream);               \
    fprintf(stream, __VA_ARGS__);               \
    fputs("\n", stream);                        \
  }

#define OL_LOG_INFO(...)                                \
  {                                                     \
    _OL_LOG_TEMPLATE("INFO", stdout, __VA_ARGS__);      \
  }

#define OL_LOG_ERR(...)                                 \
  {                                                     \
    _OL_LOG_TEMPLATE("ERR", stderr, __VA_ARGS__);       \
  }


#endif
