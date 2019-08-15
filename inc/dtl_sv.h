/*****************************************************************************
* \file      dtl_sv.h
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     DTL Scalar
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
#ifndef DTL_SV_H__
#define DTL_SV_H__

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include "dtl_dv.h"
#include "adt_str.h"
#include "adt_bytes.h"
#include "adt_bytearray.h"
#include "dtl_error.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define DTL_SV_TYPE_MASK      0xF0
#define DTL_SV_TYPE_SHIFT     4

typedef struct dtl_pv_tag{
   void *p;
   void (*pDestructor)(void*);
}dtl_pv_t;


typedef union dtl_sv_value_tag{
    int32_t    i32;
    uint32_t   u32;
    int64_t    i64;
    uint64_t   u64;
    float      flt;
    double     dbl;
    adt_str_t  *str;
    dtl_pv_t   ptr;
    dtl_dv_t*  dv;
    bool       bl;
    adt_bytes_t *bytes;
    adt_bytearray_t *bytearray;
} dtl_sv_value_t;

typedef struct dtl_svx_tag
{
   adt_str_t *tmpStr; //used as temporary storage area when user calls dtl_sv_to_cstr
   dtl_sv_value_t val;
} dtl_svx_t;


typedef struct dtl_sv_tag{
   DTL_DV_HEAD(dtl_svx_t)
}dtl_sv_t;

//TODO: Right now we can only have 16 different types of scalar values. Change typeId to use 8 bits out of u32Flags instead of 4 bits.
typedef enum dtl_sv_type_tag{
   DTL_SV_NONE = 0,
   DTL_SV_I32,
   DTL_SV_U32,
   DTL_SV_I64,
   DTL_SV_U64,
   DTL_SV_FLT,
   DTL_SV_DBL,
   DTL_SV_BOOL,
   DTL_SV_STR,
   DTL_SV_PTR,
   DTL_SV_DV,
   DTL_SV_BYTES,
   DTL_SV_BYTEARRAY
} dtl_sv_type_id;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
extern dtl_sv_t g_dtl_sv_none;
//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
//forward declarations
struct dtl_av_tag;
struct dtl_hv_tag;

//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void);
void dtl_sv_delete(dtl_sv_t* self);
void dtl_sv_create(dtl_sv_t* self);
void dtl_sv_destroy(dtl_sv_t* self);
dtl_sv_t *dtl_sv_make_i32(int32_t i32);
dtl_sv_t *dtl_sv_make_u32(uint32_t u32);
dtl_sv_t *dtl_sv_make_i64(int64_t i64);
dtl_sv_t *dtl_sv_make_u64(uint64_t u64);
dtl_sv_t *dtl_sv_make_flt(float sf);
dtl_sv_t *dtl_sv_make_dbl(double df);
dtl_sv_t *dtl_sv_make_bool(bool bl);
dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*));
dtl_sv_t *dtl_sv_make_str(const adt_str_t *str);
dtl_sv_t *dtl_sv_make_cstr(const char* cstr);
dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv, bool autoIncRef);
dtl_sv_t *dtl_sv_make_bytes(adt_bytes_t *bytes);
dtl_sv_t *dtl_sv_make_bytes_raw(const uint8_t *dataBuf, uint32_t dataLen);
dtl_sv_t *dtl_sv_make_bytearray(adt_bytearray_t *array);
dtl_sv_t *dtl_sv_make_bytearray_raw(const uint8_t *dataBuf, uint32_t dataLen);

//getters
dtl_sv_type_id dtl_sv_type(const dtl_sv_t* self);
dtl_dv_type_id dtl_sv_dv_type(const dtl_sv_t* self);
const adt_bytes_t* dtl_sv_get_bytes(const dtl_sv_t* self); //Gets a read-only copy, use dtl_sv_to_bytes in order to get a cloned object
const adt_bytearray_t* dtl_sv_get_bytearray(const dtl_sv_t* self); //Gets a read-only copy, use dtl_sv_to_bytearray in order to get a cloned object


//Setters
void dtl_sv_set_i32(dtl_sv_t *self, int32_t i32);
void dtl_sv_set_u32(dtl_sv_t *self, uint32_t u32);
void dtl_sv_set_i64(dtl_sv_t *self, int64_t i64);
void dtl_sv_set_u64(dtl_sv_t *self, uint64_t u64);
void dtl_sv_set_flt(dtl_sv_t *self, float flt);
void dtl_sv_set_dbl(dtl_sv_t *self, double dbl);
void dtl_sv_set_bool(dtl_sv_t *self, bool bl);
void dtl_sv_set_ptr(dtl_sv_t *self, void *p, void (*pDestructor)(void*));
void dtl_sv_set_str(dtl_sv_t *self, const adt_str_t *str);
void dtl_sv_set_cstr(dtl_sv_t *self, const char* str);
void dtl_sv_set_bstr(dtl_sv_t *self, const uint8_t *pBegin, const uint8_t *pEnd);
void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv, bool autoIncRef);
void dtl_sv_set_bytes(dtl_sv_t *self, adt_bytes_t *bytes);
void dtl_sv_set_bytes_raw(dtl_sv_t *self, const uint8_t *dataBuf, uint32_t dataLen);
void dtl_sv_set_bytearray(dtl_sv_t *self, adt_bytearray_t *array);
void dtl_sv_set_bytearray_raw(dtl_sv_t *self, const uint8_t *dataBuf, uint32_t dataLen);

//Conversion functions
int32_t dtl_sv_to_i32(const dtl_sv_t *self, bool *ok);
uint32_t dtl_sv_to_u32(const dtl_sv_t *self, bool *ok);
int64_t dtl_sv_to_i64(const dtl_sv_t *self, bool *ok);
uint64_t dtl_sv_to_u64(const dtl_sv_t *self, bool *ok);
float dtl_sv_to_flt(const dtl_sv_t *self, bool *ok);
double dtl_sv_to_dbl(const dtl_sv_t *self, bool *ok);
bool dtl_sv_to_bool(const dtl_sv_t *self);
void* dtl_sv_to_ptr(const dtl_sv_t *self);
adt_str_t *dtl_sv_to_str(const dtl_sv_t *self);
const char *dtl_sv_to_cstr(dtl_sv_t *self);
dtl_dv_t *dtl_sv_to_dv(const dtl_sv_t *self);
dtl_sv_t *dtl_sv_to_sv(const dtl_sv_t *self);
struct dtl_av_tag *dtl_sv_to_av(const dtl_sv_t *self);
struct dtl_hv_tag *dtl_sv_to_hv(const dtl_sv_t *self);

//Comparison functions
dtl_error_t dtl_sv_lt(const dtl_sv_t *self, const dtl_sv_t *other, bool *result);

//Macros
#define dtl_sv_none() &g_dtl_sv_none

#endif //DTL_SV_H__

