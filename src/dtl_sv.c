/*****************************************************************************
* \file      dtl_sv.c
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     DTL Scalar
*
* Copyright (c) 2013-2021 Conny Gustafsson
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

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stddef.h>
#include "dtl_dv.h"
#include "dtl_sv.h"
#include "dtl_av.h"
#include "dtl_hv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

#ifdef _WIN64
#include <Basetsd.h>
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_BUF 128
#define BYTEARRAY_DEFAULT_GROWSIZE 256
#define DTL_CHAR_MIN -128
#define DTL_CHAR_MAX 127

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void dtl_sv_set_type(dtl_sv_t *self,dtl_sv_type_id type);
static void dtl_sv_ztrim(char *str);
static void dtl_sv_to_string_internal(const dtl_sv_t *self, adt_str_t* str, bool* ok);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
static dtl_svx_t g_dtl_svx_none = {0};
dtl_sv_t g_dtl_sv_none = {&g_dtl_svx_none, 1, ((uint32_t)DTL_DV_SCALAR)};

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void)
{
   dtl_sv_t *self = (dtl_sv_t*) malloc(sizeof(dtl_sv_t));
   if(self !=(dtl_sv_t*)0)
   {
      dtl_sv_create(self);
   }
   return self;
}

void dtl_sv_delete(dtl_sv_t* self)
{
   if(self){
      dtl_sv_destroy(self);
      free(self);
   }
}

void dtl_sv_create(dtl_sv_t* self)
{
   if(self)
   {
      self->pAny = (dtl_svx_t*) malloc(sizeof(dtl_svx_t));
      if (self->pAny != 0)
      {
         memset(self->pAny, 0, sizeof(dtl_svx_t));
         self->u32Flags = ((uint32_t)DTL_DV_SCALAR);
         self->u32RefCnt = 1;
         self->pAny->tmpStr = (adt_str_t*) 0;
      }
      else
      {
         assert(0);
      }
   }
}

void dtl_sv_destroy(dtl_sv_t* self)
{
   if(self != 0)
   {
      switch(dtl_sv_type(self))
      {
      case DTL_SV_STR:
         adt_str_delete(self->pAny->val.str);
         break;
      case DTL_SV_PTR:
         if(self->pAny->val.ptr.pDestructor != 0)
         {
            self->pAny->val.ptr.pDestructor(self->pAny->val.ptr.p);
         }
         break;
      case DTL_SV_DV:
         if(self->pAny->val.dv != 0)
         {
            dtl_dv_dec_ref(self->pAny->val.dv);
         }
         break;
      case DTL_SV_BYTES:
         adt_bytes_delete(self->pAny->val.bytes);
         break;
      case DTL_SV_BYTEARRAY:
         adt_bytearray_delete(self->pAny->val.bytearray);
         break;
      default:
         break;
      }
      if (self->pAny->tmpStr != 0)
      {
         adt_str_delete(self->pAny->tmpStr);
         self->pAny->tmpStr = (adt_str_t*) 0;
      }
      free(self->pAny);
      self->pAny = 0;
   }
}

dtl_sv_t *dtl_sv_make_i32(int32_t i32){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_i32(self,i32);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_u32(uint32_t u32){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_u32(self,u32);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_i64(int64_t i64){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_i64(self,i64);
   }
   return self;
}
dtl_sv_t *dtl_sv_make_u64(uint64_t u64){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_u64(self,u64);
   }
   return self;
}


dtl_sv_t *dtl_sv_make_flt(float flt){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_flt(self,flt);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_dbl(double dbl){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_dbl(self,dbl);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bool(bool bl){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_bool(self,bl);
   }
   return self;
}

dtl_sv_t* dtl_sv_make_char(char cr)
{
   dtl_sv_t* self = dtl_sv_new();
   if (self) {
      dtl_sv_set_char(self, cr);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*)){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_ptr(self,ptr,pDestructor);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_str(const adt_str_t *str)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_str(self, str);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_cstr(const char* cstr){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_cstr(self, cstr);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv, bool autoIncRef)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self)
   {
      dtl_sv_set_dv(self, dv, autoIncRef);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytes(adt_bytes_t *bytes)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self)
   {
      dtl_sv_set_bytes(self, bytes);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytes_raw(const uint8_t *dataBuf, uint32_t dataLen)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self)
   {
      dtl_sv_set_bytes_raw(self, dataBuf, dataLen);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytearray(adt_bytearray_t *array)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self)
   {
      dtl_sv_set_bytearray(self, array);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytearray_raw(const uint8_t *dataBuf, uint32_t dataLen)
{
   dtl_sv_t *self = dtl_sv_new();
   if(self)
   {
      dtl_sv_set_bytearray_raw(self, dataBuf, dataLen);
   }
   return self;
}

dtl_sv_type_id dtl_sv_type(const dtl_sv_t* self){
   if(self){
      uint8_t u8Type = (uint8_t) ((self->u32Flags & DTL_SV_TYPE_MASK)>>DTL_SV_TYPE_SHIFT);
      return (dtl_sv_type_id) u8Type;
   }
   return DTL_SV_NONE;
}

dtl_dv_type_id dtl_sv_dv_type(const dtl_sv_t* self){
   if( (self != 0) && ( (dtl_sv_type(self) == DTL_SV_DV)) ){
      return dtl_dv_type(self->pAny->val.dv);
   }
   return DTL_DV_INVALID;
}


//Setters
void dtl_sv_set_i32(dtl_sv_t *self, int32_t i32){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_I32);
      self->pAny->val.i32 = i32;
   }
}

void dtl_sv_set_u32(dtl_sv_t *self, uint32_t u32){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_U32);
      self->pAny->val.u32 = u32;
   }
}

void dtl_sv_set_i64(dtl_sv_t *self, int64_t i64){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_I64);
      self->pAny->val.i64 = i64;
   }
}

void dtl_sv_set_u64(dtl_sv_t *self, uint64_t u64){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_U64);
      self->pAny->val.u64 = u64;
   }
}


void dtl_sv_set_flt(dtl_sv_t *self, float flt){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_FLT);
      self->pAny->val.flt = flt;
   }
}
void dtl_sv_set_dbl(dtl_sv_t *self, double dbl){
   if(self){
      dtl_sv_set_type(self,DTL_SV_DBL);
      self->pAny->val.dbl = dbl;
   }
}

void dtl_sv_set_bool(dtl_sv_t *self, bool bl){
   if(self){
      dtl_sv_set_type(self,DTL_SV_BOOL);
      self->pAny->val.bl = bl;
   }
}

void dtl_sv_set_char(dtl_sv_t* self, char cr)
{
   if (self)
   {
      dtl_sv_set_type(self, DTL_SV_CHAR);
      self->pAny->val.cr = cr;
   }
}

void dtl_sv_set_ptr(dtl_sv_t *self, void *p, void (*pDestructor)(void*)){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_dec_ref(self->pAny->val.dv);
      }
      dtl_sv_set_type(self,DTL_SV_PTR);
      self->pAny->val.ptr.p = p;
      self->pAny->val.ptr.pDestructor = pDestructor;
   }
}

void dtl_sv_set_str(dtl_sv_t *self, const adt_str_t *str)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set(self->pAny->val.str, str);
   }
}

void dtl_sv_set_cstr(dtl_sv_t *self, const char* cstr){
   if(self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_cstr(self->pAny->val.str, cstr);
   }
}

void dtl_sv_set_bstr(dtl_sv_t *self, const uint8_t *pBegin, const uint8_t *pEnd)
{
   if ( (self != 0) && (pBegin != 0) && (pEnd != 0) && (pBegin<=pEnd) )
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_bstr(self->pAny->val.str, pBegin, pEnd);
   }
}

void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv, bool autoIncRef)
{
   if(self != 0)
   {
      dtl_sv_set_type(self,DTL_SV_DV);
      self->pAny->val.dv = dv;
      if (autoIncRef)
      {
         dtl_dv_inc_ref(dv);
      }
   }
}

void dtl_sv_set_bytes(dtl_sv_t *self, adt_bytes_t *bytes)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = adt_bytes_clone(bytes);
   }
}

void dtl_sv_set_bytes_raw(dtl_sv_t *self, const uint8_t *dataBuf, uint32_t dataLen)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = adt_bytes_new(dataBuf, dataLen);
   }
}

void dtl_sv_set_bytearray(dtl_sv_t *self, adt_bytearray_t *array)
{
   dtl_sv_set_type(self, DTL_SV_BYTEARRAY);
   adt_bytearray_append(self->pAny->val.bytearray, array->pData, array->u32CurLen);
}

void dtl_sv_set_bytearray_raw(dtl_sv_t *self, const uint8_t *dataBuf, uint32_t dataLen)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_BYTEARRAY);
      adt_bytearray_append(self->pAny->val.bytearray, dataBuf, dataLen);
   }
}

void dtl_sv_take_bytes(dtl_sv_t *self, adt_bytes_t *bytes)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = bytes;
   }
}


//Getters
int32_t dtl_sv_to_i32(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   int32_t retval = 0;
   if(self != 0)
   {
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         if (self->pAny->val.u32 <= INT32_MAX)
         {
            retval = (int32_t)self->pAny->val.u32;
            success = true;
         }
         break;
      case DTL_SV_I64:
         if ( (self->pAny->val.i64 >= INT32_MIN) && (self->pAny->val.i64 <= INT32_MAX))
         {
            retval = (int32_t)self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.i64 <= INT32_MAX)
         {
            retval = (int32_t)self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_FLT:
         if ((self->pAny->val.flt >= (float)INT32_MIN) && (self->pAny->val.flt <= (float)INT32_MAX))
         {
            retval = (int32_t)self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if ((self->pAny->val.dbl >= (double)INT32_MIN) && (self->pAny->val.dbl <= (double)INT32_MAX))
         {
            retval = (int32_t)self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (int32_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (int32_t)self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}
uint32_t dtl_sv_to_u32(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   uint32_t retval = 0u;
   if(self)
   {
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         if (self->pAny->val.i32 >= 0)
         {
            retval = (uint32_t)self->pAny->val.i32;
            success = true;
         }
         break;
      case DTL_SV_U32:
         retval =  self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         if ( (self->pAny->val.i64 >= 0) && (self->pAny->val.i64 <= (int64_t)UINT32_MAX))
         {
            retval = (uint32_t)self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= (uint64_t)UINT32_MAX)
         {
            retval = (uint32_t)self->pAny->val.u64;
            success = true;
         }
         success = true;
         break;
      case DTL_SV_FLT:
         if (self->pAny->val.flt >= 0.0)
         {
            retval = (uint32_t)self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if (self->pAny->val.dbl >= 0.0)
         {
            retval = (uint32_t)self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (uint32_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (uint32_t)self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}

int64_t dtl_sv_to_i64(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   int64_t retval = 0u;

   if(self != 0)
   {
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = (int32_t)self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (int64_t) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (int64_t)self->pAny->val.i64;
         success = true;
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= INT64_MAX)
         {
            retval = (int64_t)self->pAny->val.i64;
            success = true;
         }
         break;
     case DTL_SV_FLT:
         retval = (int64_t) self->pAny->val.flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (int64_t) self->pAny->val.dbl;
         success = true;
         break;
     case DTL_SV_BOOL:
         retval = (int64_t) self->pAny->val.bl;
         success = true;
         break;
     case DTL_SV_CHAR:
        retval = (int64_t)self->pAny->val.cr;
        success = true;
        break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}

uint64_t dtl_sv_to_u64(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   uint64_t retval = 0u;

   if(self != 0)
   {
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         if (self->pAny->val.i32 >= 0)
         {
            retval = (uint64_t)self->pAny->val.i32;
            success = true;
         }
         break;         break;
      case DTL_SV_U32:
         retval = (uint64_t) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         if (self->pAny->val.i32 >= 0)
         {
            retval = (uint64_t)self->pAny->val.i64;
            success = true;
         }
         break;         break;
      case DTL_SV_U64:
         retval = self->pAny->val.u64;
         success = true;
         break;
      case DTL_SV_FLT:
         if (self->pAny->val.flt >= 0.0)
         {
            retval = (uint64_t)self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if (self->pAny->val.dbl >= 0.0)
         {
            retval = (uint64_t)self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (uint64_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (uint64_t)self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}


float dtl_sv_to_flt(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   float retval = 0u;

   if(self != 0)
   {
      switch(dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = (float) self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (float) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (float) self->pAny->val.i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (float) self->pAny->val.u64;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = self->pAny->val.flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (float) self->pAny->val.dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (float) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (float)self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}

double dtl_sv_to_dbl(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   double retval = 0u;

   if(self != 0)
   {
      switch(dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = (double) self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (double) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = (double) self->pAny->val.flt;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (double) self->pAny->val.i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (double) self->pAny->val.u64;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = self->pAny->val.dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (double) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (double)self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}

char dtl_sv_to_char(const dtl_sv_t *self, bool *ok){
   if( self != NULL ) {
      bool success = false;
      char retval = 0;
      if (ok != NULL) *ok = true;
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_I32:
         if ((self->pAny->val.i32 >= DTL_CHAR_MIN) && (self->pAny->val.i32 <= DTL_CHAR_MAX))
         {
            retval = (char)self->pAny->val.i32;
            success = true;
         }
         break;
      case DTL_SV_U32:
         if (self->pAny->val.u32 <= DTL_CHAR_MAX)
         {
            retval = (char)self->pAny->val.u32;
            success = true;
         }
         break;
      case DTL_SV_I64:
         if ((self->pAny->val.i64 >= DTL_CHAR_MIN) && (self->pAny->val.i64 <= DTL_CHAR_MAX))
         {
            retval = (char)self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= DTL_CHAR_MAX)
         {
            retval = (char)self->pAny->val.u64;
            success = true;
         }
         break;
      case DTL_SV_CHAR:
         retval = self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_FLT:
         if ((self->pAny->val.flt >= -128.0) && (self->pAny->val.i64 <= 127.0))
         {
            retval = (char)self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if ((self->pAny->val.dbl >= -128.0) && (self->pAny->val.dbl <= 127.0))
         {
            retval = (char)self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = self->pAny->val.bl? 1 : 0;
         break;

      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_DV:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_BYTES:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_BYTEARRAY:
         if (ok != NULL) *ok = false;
         break;
      }
      if (ok != 0)
      {
         *ok = success;
      }
      return retval;
   }
   return false;
}

bool dtl_sv_to_bool(const dtl_sv_t* self, bool* ok) {
   if (self != NULL){
      if (ok != NULL) *ok = true;
      bool retval = false;
      switch (dtl_sv_type(self)) {
      case DTL_SV_NONE:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_I32:
         retval = (bool)self->pAny->val.i32;
         break;
      case DTL_SV_U32:
         retval = (bool)self->pAny->val.u32;
         break;
      case DTL_SV_I64:
         retval = (bool)self->pAny->val.i64;
         break;
      case DTL_SV_U64:
         retval = (bool)self->pAny->val.u64;
         break;
      case DTL_SV_FLT:
         retval = (bool)self->pAny->val.flt;
         break;
      case DTL_SV_DBL:
         retval = (bool)self->pAny->val.dbl;
         break;
      case DTL_SV_BOOL:
         retval = self->pAny->val.bl;
         break;
      case DTL_SV_STR:
         if (adt_str_equal_cstr(self->pAny->val.str, "true") || adt_str_equal_cstr(self->pAny->val.str, "TRUE")) {
            retval = true;
         }
         else if (!adt_str_equal_cstr(self->pAny->val.str, "false") && !adt_str_equal_cstr(self->pAny->val.str, "FALSE")) {
            if (ok != NULL) *ok = false;
         }
         break;
      case DTL_SV_PTR:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_DV:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_BYTES:
         if (ok != NULL) *ok = false;
         break;
      case DTL_SV_BYTEARRAY:
         if (ok != NULL) *ok = false;
         break;
      }
      return retval;
   }
   return false;
}

void* dtl_sv_to_ptr(const dtl_sv_t *self){
   if(self){
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
#ifdef _WIN64
         return IntToPtr(self->pAny->val.i32);
#else
         return (void*) ((long) self->pAny->val.i32);
#endif
      case DTL_SV_U32:
#ifdef _WIN64
         return UIntToPtr(self->pAny->val.u32);
#else
         return (void*) ((unsigned long) self->pAny->val.u32);
#endif
      case DTL_SV_I64:
         break;
      case DTL_SV_U64:
         break;
      case DTL_SV_FLT:
         break;
      case DTL_SV_DBL:
         break;
      case DTL_SV_BOOL:
         break;
      case DTL_SV_STR:
         return (void*) &self->pAny->val.str;
         break;
      case DTL_SV_PTR:
         return self->pAny->val.ptr.p;
         break;
      case DTL_SV_DV:
         return (void*) self->pAny->val.dv;
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   return (void*)0;
}

const char* dtl_sv_to_cstr(dtl_sv_t *self, bool* ok){
   if(self != NULL)
   {
      if (ok != NULL) *ok = false;
      switch(dtl_sv_type(self)){
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
      case DTL_SV_U32:
      case DTL_SV_I64:
      case DTL_SV_U64:
      case DTL_SV_FLT:
      case DTL_SV_DBL:
      case DTL_SV_CHAR:
         if (self->pAny->tmpStr == NULL)
         {
            self->pAny->tmpStr = adt_str_new();
         }
         else
         {
            adt_str_clear(self->pAny->tmpStr);
         }
         if (self->pAny->tmpStr != 0)
         {
            dtl_sv_to_string_internal(self, self->pAny->tmpStr, ok);
            return adt_str_cstr(self->pAny->tmpStr);
         }
         break;
      case DTL_SV_BOOL:
         if (ok != NULL) *ok = true;
         return self->pAny->val.bl? "true" : "false";
      case DTL_SV_STR:
         if (ok != NULL) *ok = true;
         return adt_str_cstr(self->pAny->val.str);
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      case DTL_SV_BYTES:
         break;
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   return NULL;
}

adt_str_t* dtl_sv_to_str(const dtl_sv_t *self, bool* ok)
{
   adt_str_t *str = (adt_str_t*) 0;
   if(self != 0)
   {
      str = adt_str_new();
      if (str != 0)
      {
         dtl_sv_to_string_internal(self, str, ok);
      }
   }
   return str;
}

dtl_dv_t *dtl_sv_to_dv(const dtl_sv_t *self)
{
   if(self != 0)
   {
      if(dtl_sv_type(self) == DTL_SV_DV)
      {
         return self->pAny->val.dv;
      }
   }
   return (dtl_dv_t*) 0;
}

dtl_sv_t *dtl_sv_to_sv(const dtl_sv_t *self)
{
   if(self != 0)
   {
      if(dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_SCALAR )
         {
            return (dtl_sv_t*) dv;
         }
      }
   }
   return (dtl_sv_t*) 0;
}


struct dtl_av_tag *dtl_sv_to_av(const dtl_sv_t *self)
{
   if(self != 0)
   {
      if(dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_ARRAY )
         {
            return (dtl_av_t*) dv;
         }
      }
   }
   return (dtl_av_t*) 0;
}

struct dtl_hv_tag *dtl_sv_to_hv(const dtl_sv_t *self)
{
   if(self != 0)
   {
      if(dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_HASH )
         {
            return (dtl_hv_t*) dv;
         }
      }
   }
   return (dtl_hv_t*) 0;
}

//Comparison functions

/*
 * Compares if self is less than other.
 * If function returns DTL_NO_ERROR the result (of the compariosn) is placed in the result argument.
 */
dtl_error_t dtl_sv_lt(const dtl_sv_t *self, const dtl_sv_t *other, bool *result)
{
   if ( (self != 0) && (other != 0) && (result != 0) )
   {
      dtl_error_t retval = DTL_TYPE_ERROR;
      dtl_sv_type_id leftType, rightType;
      *result = false;
      leftType = dtl_sv_type(self);
      rightType = dtl_sv_type(other);
      switch(leftType)
      {
      case DTL_SV_NONE:
         retval = DTL_TYPE_ERROR; //cannot compare if None is less than None
         break;
      case DTL_SV_I32:
         if (rightType == DTL_SV_I32)
         {
            *result = self->pAny->val.i32 < other->pAny->val.i32;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_U32:
         if (rightType == DTL_SV_U32)
         {
            *result = self->pAny->val.u32 < other->pAny->val.u32;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_I64:
         if (rightType == DTL_SV_I64)
         {
            *result = self->pAny->val.i64 < other->pAny->val.i64;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_U64:
         if (rightType == DTL_SV_U64)
         {
            *result = self->pAny->val.u64 < other->pAny->val.u64;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_FLT:
         if (rightType == DTL_SV_FLT)
         {
            *result = self->pAny->val.flt < other->pAny->val.flt;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_DBL:
         if (rightType == DTL_SV_DBL)
         {
            *result = self->pAny->val.dbl < other->pAny->val.dbl;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_BOOL:
         if (rightType == DTL_SV_BOOL)
         {
            *result = self->pAny->val.bl < other->pAny->val.bl;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_STR:
         if (rightType == DTL_SV_STR)
         {
            int tmp = adt_str_lt(self->pAny->val.str, other->pAny->val.str);
            if (tmp >= 0)
            {
               *result = (bool) tmp;
               retval = DTL_NO_ERROR;
            }
         }
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      default:
         retval = DTL_TYPE_ERROR;
      }
      return retval;
   }
   return DTL_INVALID_ARGUMENT_ERROR;
}

const adt_bytes_t* dtl_sv_get_bytes(const dtl_sv_t* self)
{
   const adt_bytes_t *retval = (const adt_bytes_t*) 0;
   if (self != 0)
   {
      dtl_sv_type_id currentType = dtl_sv_type(self);
      if (currentType == DTL_SV_BYTES)
      {
         retval = self->pAny->val.bytes;
      }
   }
   return retval;
}

const adt_bytearray_t* dtl_sv_get_bytearray(const dtl_sv_t* self)
{
   const adt_bytearray_t *retval = (const adt_bytearray_t*) 0;
   if (self != 0)
   {
      dtl_sv_type_id currentType = dtl_sv_type(self);
      if (currentType == DTL_SV_BYTEARRAY)
      {
         retval = self->pAny->val.bytearray;
      }
   }
   return retval;
}



//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
static void dtl_sv_set_type(dtl_sv_t *self, dtl_sv_type_id newType)
{
   dtl_sv_type_id currentType = dtl_sv_type(self);
   if(currentType == DTL_SV_DV)
   {
      dtl_dv_dec_ref(self->pAny->val.dv);
   }
   else if (currentType == DTL_SV_STR)
   {
      if (newType != DTL_SV_STR)
      {
         adt_str_delete(self->pAny->val.str);
      }
   }
   else if (currentType == DTL_SV_BYTES)
   {
      adt_bytes_delete(self->pAny->val.bytes);
      self->pAny->val.bytes = (adt_bytes_t*) 0;
   }
   else if (currentType == DTL_SV_BYTEARRAY)
   {
      if (newType != DTL_SV_BYTEARRAY)
      {
         adt_bytearray_delete(self->pAny->val.bytearray);
         self->pAny->val.bytearray = (adt_bytearray_t*) 0;
      }
   }
   else
   {

   }

   if (newType == DTL_SV_STR)
   {
      if (currentType != DTL_SV_STR)
      {
         self->pAny->val.str = adt_str_new();
      }
      else
      {
         adt_str_clear(self->pAny->val.str);
      }
   }
   else if(newType == DTL_SV_BYTEARRAY)
   {
      if (currentType != DTL_SV_BYTEARRAY)
      {
         self->pAny->val.bytearray = adt_bytearray_new(BYTEARRAY_DEFAULT_GROWSIZE);
      }
      else
      {
         adt_bytearray_clear(self->pAny->val.bytearray);
      }
   }

   self->u32Flags &= ~((uint32_t)DTL_SV_TYPE_MASK);
   self->u32Flags |= (((uint32_t)newType)<<DTL_SV_TYPE_SHIFT) & DTL_SV_TYPE_MASK;
}

static void dtl_sv_ztrim(char *str)
{
   char *begin = str;
   char *end = begin+strlen(str);
   char *p = begin;
   char *a=0,*b=0;
   while( p<end){
      if(!isdigit(*p)){
         if(*p == '.'){
            a = p;
            break;
         }
         else{
            return; //not a number
         }
      }
      ++p;
   }
   p=end-1;
   while(p>a){
      if(*p == '0'){
         b=p;
      }
      else{
         break;
      }
      --p;
   }
   if(a && b){
      assert(b>a);
      if(b-1==a){
         --b; //nothing on the right side of the '.'
      }
      *b=0;
   }
   if(strlen(str)==0){
      //empty string, replace with 0
      strcpy(str,"0");
   }
}

static void dtl_sv_to_string_internal(const dtl_sv_t* self, adt_str_t* str, bool* ok)
{
   char numBuf[MAX_NUM_BUF];
   bool isNum = false;
   switch(dtl_sv_type(self))
   {
   case DTL_SV_NONE:
      if (ok != NULL) *ok = true;
      adt_str_append_cstr(str,"(undefined)");
      break;
   case DTL_SV_I32:
   case DTL_SV_CHAR:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%d", (int) dtl_sv_to_i32(self, NULL));
      isNum = true;
      break;
   case DTL_SV_U32:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%u", (unsigned int) dtl_sv_to_u32(self, NULL));
      isNum = true;
      break;
   case DTL_SV_I64:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%lld", (long long int) dtl_sv_to_i64(self, NULL));
      isNum = true;
      break;
   case DTL_SV_U64:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%llu", (long long unsigned int) dtl_sv_to_u64(self, NULL));
      isNum = true;
      break;
   case DTL_SV_FLT:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%f", (double) dtl_sv_to_flt(self, NULL));
      isNum = true;
      dtl_sv_ztrim(numBuf);
      break;
   case DTL_SV_DBL:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%f", dtl_sv_to_dbl(self, NULL));
      isNum = true;
      dtl_sv_ztrim(numBuf);
      break;
   case DTL_SV_BOOL:
      if (ok != NULL) *ok = true;
      if(dtl_sv_to_bool(self, NULL))
      {
         adt_str_set_cstr(str, "true");
      }
      else
      {
         adt_str_set_cstr(str, "false");
      }
      break;
   case DTL_SV_STR:
      if (ok != NULL) *ok = true;
      adt_str_set(str, self->pAny->val.str);
      break;
   case DTL_SV_PTR:
      if (ok != NULL) *ok = true;
      sprintf(numBuf,"%p", dtl_sv_to_ptr(self));
      isNum = true;
      break;
   case DTL_SV_DV:
      //No conversion available
      break;
   case DTL_SV_BYTES:
      //No conversion available
      break;
   case DTL_SV_BYTEARRAY:
      //No conversion available
      break;
   }
   if(isNum)
   {
      adt_str_set_cstr(str, numBuf);
   }
}
