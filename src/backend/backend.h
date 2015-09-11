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

#ifndef _OPENLUX_BACKEND_H
#define _OPENLUX_BACKEND_H


#include "../util.h"


#define _OL_BACKEND_JOIN(x, y) OL_UTIL_CAT(OL_UTIL_CAT(x, _), y)
#define _OL_BACKEND_NAME_PREFIX() _OL_BACKEND_JOIN(ol_backend, OL_BACKEND_PREFIX)
#define _OL_BACKEND_NAME_SUBPREFIX(sub) _OL_BACKEND_JOIN(_OL_BACKEND_NAME_PREFIX(), sub)
#define _OL_BACKEND_NAME_SUBINIT(sub) _OL_BACKEND_JOIN(_OL_BACKEND_NAME_SUBPREFIX(sub), init)
#define _OL_BACKEND_NAME_LIST() _OL_BACKEND_JOIN(_OL_BACKEND_NAME_PREFIX(), list)
#define _OL_BACKEND_NAME_STRUCT() _OL_BACKEND_JOIN(_OL_BACKEND_NAME_PREFIX(), s)
#define _OL_BACKEND_NAME_DATA() _OL_BACKEND_JOIN(_OL_BACKEND_NAME_PREFIX(), data_s)
#define _OL_BACKEND_STRUCT() struct _OL_BACKEND_NAME_STRUCT()

#define OL_BACKEND_INIT(sub) extern struct _OL_BACKEND_NAME_STRUCT() _OL_BACKEND_NAME_SUBPREFIX(sub)

#define OL_BACKEND_LIST() static _OL_BACKEND_STRUCT()* _OL_BACKEND_NAME_LIST()[]
  ; /* emacs */
#define OL_BACKEND_LIST_ITEM(sub) &_OL_BACKEND_NAME_SUBPREFIX(sub)
#define OL_BACKEND_LIST_FAIL() ((_OL_BACKEND_STRUCT()*)1)
#define OL_BACKEND_LIST_END() 0

#define OL_BACKEND_FIND(ret)                                            \
  {                                                                     \
    ret = -2;                                                           \
    _OL_BACKEND_STRUCT()** i = _OL_BACKEND_NAME_LIST();                 \
    if (index >= 0)                                                     \
      i += index;                                                       \
    for (; *i; i++)                                                     \
      {                                                                 \
        if (*i == OL_BACKEND_LIST_FAIL())                               \
          continue;                                                     \
        *self = **i;                                                    \
        self->data = data;                                              \
        (ret) = (*i)->init(self);                                       \
        if (!(ret) || index >= 0)                                       \
          break;                                                        \
      }                                                                 \
  }

#define OL_BACKEND_DATA_INIT(data) struct _OL_BACKEND_NAME_DATA()* data = malloc(sizeof(struct _OL_BACKEND_NAME_DATA()))

#define OL_BACKEND_DATA()                                               \
  ((struct _OL_BACKEND_NAME_DATA()*)(self->data))

#endif
