/*****************************************************************************
* \file      dtl_type.h
* \author    Conny Gustafsson
* \date      2013-07-27
* \brief     DTL type
*
* Copyright (c) 2013-2019 Conny Gustafsson
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
******************************************************************************/
#ifndef DTL_TYPE_H_
#define DTL_TYPE_H_

//////////////////////////////////////////////////////////////////////////////
// VERSION
//////////////////////////////////////////////////////////////////////////////

#define _xstr(s) _str(s)
#define _str(s) #s
#define _DTL_MAKE_VERSION(x, y, z) _xstr(x) "." _xstr(y) "." _xstr(z)

#define DTL_VERSION_MAJOR 0
#define DTL_VERSION_MINOR 1
#define DTL_VERSION_PATCH 0

#define DTL_TYPE_VERSION _DTL_MAKE_VERSION(DTL_VERSION_MAJOR, DTL_VERSION_MINOR, DTL_VERSION_PATCH)

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dtl_sv.h"
#include "dtl_av.h"
#include "dtl_hv.h"

#endif //DTL_TYPE_H_
