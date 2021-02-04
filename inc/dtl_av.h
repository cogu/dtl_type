/*****************************************************************************
* \file      dtl_av.h
* \author    Conny Gustafsson
* \date      2013-03-10
* \brief     DTL Array
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
#ifndef DTL_AV_H__
#define DTL_AV_H__

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "dtl_dv.h"
#include "adt_ary.h"
#include "dtl_error.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

typedef struct dtl_av_tag{
  DTL_DV_HEAD(adt_ary_t)
} dtl_av_t;

typedef dtl_dv_t* (dtl_key_func_t)(const dtl_dv_t *dv);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
dtl_av_t* dtl_av_new();
dtl_av_t* dtl_av_make(dtl_dv_t** ppValue, int32_t s32Len);
void dtl_av_delete(dtl_av_t *self);
void dtl_av_create(dtl_av_t *self);
void dtl_av_destroy(dtl_av_t *self);

//Accessors
dtl_dv_t** dtl_av_set(dtl_av_t *self, int32_t s32Index, dtl_dv_t *pValue);
dtl_dv_t** dtl_av_get(const dtl_av_t *self, int32_t s32Index);
void dtl_av_push(dtl_av_t *self, dtl_dv_t *dv, bool autoIncrementRef);
dtl_dv_t* dtl_av_pop(dtl_av_t *self);
dtl_dv_t* dtl_av_shift(dtl_av_t *self);
void dtl_av_unshift(dtl_av_t *self, dtl_dv_t *pValue);
dtl_dv_t* dtl_av_value(const dtl_av_t *self, int32_t s32Index);


//Utility functions
void dtl_av_extend(dtl_av_t *self, int32_t s32Len);
void dtl_av_fill(dtl_av_t *self, int32_t s32Len);
void dtl_av_clear(dtl_av_t *self);
int32_t dtl_av_length(const dtl_av_t *self);
bool dtl_av_is_empty(const dtl_av_t* self);
bool dtl_av_exists(const dtl_av_t *self, int32_t s32Index);
dtl_error_t dtl_av_sort(dtl_av_t *self, dtl_key_func_t *key, bool reverse);

#endif //DTL_AV_H__
