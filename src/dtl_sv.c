/*****************************************************************************
* \file      dtl_sv.c
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     DTL Scalar Value implementation
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include "dtl_dv.h"
#include "dtl_sv.h"
#include "dtl_av.h"
#include "dtl_hv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define MAX_NUM_BUF 128
#define DTL_CHAR_MIN (-128)
#define DTL_CHAR_MAX 127

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void dtl_sv_set_type(dtl_sv_t *self, dtl_sv_type_id new_type);
static void dtl_sv_ztrim(char *str);
static void dtl_sv_to_string_internal(const dtl_sv_t *self, adt_str_t *str, bool *ok);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
static dtl_svx_t g_dtl_svx_none = {NULL, {0}};
dtl_sv_t g_dtl_sv_none = {&g_dtl_svx_none, 1u, ((uint32_t) DTL_DV_SCALAR)};

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

// Constructor/Destructor

dtl_sv_t *dtl_sv_new(void)
{
   dtl_sv_t *self = (dtl_sv_t *) malloc(sizeof(dtl_sv_t));
   if (self != NULL)
   {
      dtl_sv_create(self);
   }
   return self;
}

void dtl_sv_delete(dtl_sv_t *self)
{
   if (self != NULL)
   {
      dtl_sv_destroy(self);
      free(self);
   }
}

void dtl_sv_create(dtl_sv_t *self)
{
   if (self != NULL)
   {
      self->pAny = (dtl_svx_t *) malloc(sizeof(dtl_svx_t));
      if (self->pAny != NULL)
      {
         memset(self->pAny, 0, sizeof(dtl_svx_t));
         self->u32Flags = ((uint32_t) DTL_DV_SCALAR);
         self->u32RefCnt = 1u;
         self->pAny->tmp_str = NULL;
      }
      else
      {
         assert(0);
      }
   }
}

void dtl_sv_destroy(dtl_sv_t *self)
{
   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_STR:
         adt_str_delete(self->pAny->val.str);
         break;
      case DTL_SV_PTR:
         if (self->pAny->val.ptr.destructor != NULL)
         {
            self->pAny->val.ptr.destructor(self->pAny->val.ptr.p);
         }
         break;
      case DTL_SV_DV:
         if (self->pAny->val.dv != NULL)
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
      if (self->pAny->tmp_str != NULL)
      {
         adt_str_delete(self->pAny->tmp_str);
         self->pAny->tmp_str = NULL;
      }
      free(self->pAny);
      self->pAny = NULL;
   }
}

dtl_sv_t *dtl_sv_make_i32(int32_t value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_i32(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_u32(uint32_t value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_u32(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_i64(int64_t value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_i64(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_u64(uint64_t value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_u64(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_flt(float value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_flt(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_dbl(double value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_dbl(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bool(bool value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_bool(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_char(char value)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_char(self, value);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_ptr(void *ptr, void (*destructor)(void *))
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_ptr(self, ptr, destructor);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_str(const adt_str_t *str)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_str(self, str);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_cstr(const char *str)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_cstr(self, str);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv, bool auto_inc_ref)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_dv(self, dv, auto_inc_ref);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytes(adt_bytes_t *bytes)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_bytes(self, bytes);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytes_raw(const uint8_t *data_buf, uint32_t data_len)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_bytes_raw(self, data_buf, data_len);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytearray(adt_bytearray_t *array)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_bytearray(self, array);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_bytearray_raw(const uint8_t *data_buf, uint32_t data_len)
{
   dtl_sv_t *self = dtl_sv_new();
   if (self != NULL)
   {
      dtl_sv_set_bytearray_raw(self, data_buf, data_len);
   }
   return self;
}

dtl_sv_type_id dtl_sv_type(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      uint8_t type_id = (uint8_t) ((self->u32Flags & DTL_SV_TYPE_MASK) >> DTL_SV_TYPE_SHIFT);
      return (dtl_sv_type_id) type_id;
   }
   return DTL_SV_NONE;
}

dtl_dv_type_id dtl_sv_dv_type(const dtl_sv_t *self)
{
   if ((self != NULL) && (dtl_sv_type(self) == DTL_SV_DV))
   {
      return dtl_dv_type(self->pAny->val.dv);
   }
   return DTL_DV_NULL;
}

// Setters

void dtl_sv_set_i32(dtl_sv_t *self, int32_t value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_I32);
      self->pAny->val.i32 = value;
   }
}

void dtl_sv_set_u32(dtl_sv_t *self, uint32_t value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_U32);
      self->pAny->val.u32 = value;
   }
}

void dtl_sv_set_i64(dtl_sv_t *self, int64_t value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_I64);
      self->pAny->val.i64 = value;
   }
}

void dtl_sv_set_u64(dtl_sv_t *self, uint64_t value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_U64);
      self->pAny->val.u64 = value;
   }
}

void dtl_sv_set_flt(dtl_sv_t *self, float value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_FLT);
      self->pAny->val.flt = value;
   }
}

void dtl_sv_set_dbl(dtl_sv_t *self, double value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_DBL);
      self->pAny->val.dbl = value;
   }
}

void dtl_sv_set_bool(dtl_sv_t *self, bool value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_BOOL);
      self->pAny->val.bl = value;
   }
}

void dtl_sv_set_char(dtl_sv_t *self, char value)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_CHAR);
      self->pAny->val.cr = value;
   }
}

void dtl_sv_set_ptr(dtl_sv_t *self, void *ptr, void (*destructor)(void *))
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_PTR);
      self->pAny->val.ptr.p = ptr;
      self->pAny->val.ptr.destructor = destructor;
   }
}

void dtl_sv_set_str(dtl_sv_t *self, const adt_str_t *str)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set(self->pAny->val.str, str);
   }
}

void dtl_sv_set_cstr(dtl_sv_t *self, const char *str)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_cstr(self->pAny->val.str, str);
   }
}

void dtl_sv_set_bstr(dtl_sv_t *self, const uint8_t *begin, const uint8_t *end)
{
   if ((self != NULL) && (begin != NULL) && (end != NULL) && (begin <= end))
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_bstr(self->pAny->val.str, begin, end);
   }
}

void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv, bool auto_inc_ref)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_DV);
      self->pAny->val.dv = dv;
      if (auto_inc_ref)
      {
         dtl_dv_inc_ref(dv);
      }
   }
}

void dtl_sv_set_bytes(dtl_sv_t *self, adt_bytes_t *bytes)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = adt_bytes_clone(bytes);
   }
}

void dtl_sv_set_bytes_raw(dtl_sv_t *self, const uint8_t *data_buf, uint32_t data_len)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = adt_bytes_new(data_buf, data_len);
   }
}

void dtl_sv_set_bytearray(dtl_sv_t *self, adt_bytearray_t *array)
{
   if ((self != NULL) && (array != NULL))
   {
      dtl_sv_set_type(self, DTL_SV_BYTEARRAY);
      adt_bytearray_append(self->pAny->val.bytearray, array->pData, array->u32CurLen);
   }
}

void dtl_sv_set_bytearray_raw(dtl_sv_t *self, const uint8_t *data_buf, uint32_t data_len)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_BYTEARRAY);
      adt_bytearray_append(self->pAny->val.bytearray, data_buf, data_len);
   }
}

void dtl_sv_take_bytes(dtl_sv_t *self, adt_bytes_t *bytes)
{
   if (self != NULL)
   {
      dtl_sv_set_type(self, DTL_SV_BYTES);
      self->pAny->val.bytes = bytes;
   }
}

// Conversion functions

int32_t dtl_sv_to_i32(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   int32_t retval = 0;
   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         if (self->pAny->val.u32 <= (uint32_t) INT32_MAX)
         {
            retval = (int32_t) self->pAny->val.u32;
            success = true;
         }
         break;
      case DTL_SV_I64:
         if ((self->pAny->val.i64 >= INT32_MIN) && (self->pAny->val.i64 <= INT32_MAX))
         {
            retval = (int32_t) self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= (uint64_t) INT32_MAX)
         {
            retval = (int32_t) self->pAny->val.u64;
            success = true;
         }
         break;
      case DTL_SV_FLT:
         if ((self->pAny->val.flt >= (float) INT32_MIN) && (self->pAny->val.flt <= (float) INT32_MAX))
         {
            retval = (int32_t) self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if ((self->pAny->val.dbl >= (double) INT32_MIN) && (self->pAny->val.dbl <= (double) INT32_MAX))
         {
            retval = (int32_t) self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (int32_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (int32_t) self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

uint32_t dtl_sv_to_u32(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   uint32_t retval = 0u;
   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         if (self->pAny->val.i32 >= 0)
         {
            retval = (uint32_t) self->pAny->val.i32;
            success = true;
         }
         break;
      case DTL_SV_U32:
         retval = self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         if ((self->pAny->val.i64 >= 0) && (self->pAny->val.i64 <= (int64_t) UINT32_MAX))
         {
            retval = (uint32_t) self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= (uint64_t) UINT32_MAX)
         {
            retval = (uint32_t) self->pAny->val.u64;
            success = true;
         }
         break;
      case DTL_SV_FLT:
         if ((self->pAny->val.flt >= 0.0f) && (self->pAny->val.flt <= (float) UINT32_MAX))
         {
            retval = (uint32_t) self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if ((self->pAny->val.dbl >= 0.0) && (self->pAny->val.dbl <= (double) UINT32_MAX))
         {
            retval = (uint32_t) self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (uint32_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         if (self->pAny->val.cr >= 0)
         {
            retval = (uint32_t) self->pAny->val.cr;
            success = true;
         }
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

int64_t dtl_sv_to_i64(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   int64_t retval = 0;

   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         retval = (int64_t) self->pAny->val.i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (int64_t) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = self->pAny->val.i64;
         success = true;
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= (uint64_t) INT64_MAX)
         {
            retval = (int64_t) self->pAny->val.u64;
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
         retval = (int64_t) self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

uint64_t dtl_sv_to_u64(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   uint64_t retval = 0u;

   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         if (self->pAny->val.i32 >= 0)
         {
            retval = (uint64_t) self->pAny->val.i32;
            success = true;
         }
         break;
      case DTL_SV_U32:
         retval = (uint64_t) self->pAny->val.u32;
         success = true;
         break;
      case DTL_SV_I64:
         if (self->pAny->val.i64 >= 0)
         {
            retval = (uint64_t) self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         retval = self->pAny->val.u64;
         success = true;
         break;
      case DTL_SV_FLT:
         if (self->pAny->val.flt >= 0.0f)
         {
            retval = (uint64_t) self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if (self->pAny->val.dbl >= 0.0)
         {
            retval = (uint64_t) self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = (uint64_t) self->pAny->val.bl;
         success = true;
         break;
      case DTL_SV_CHAR:
         if (self->pAny->val.cr >= 0)
         {
            retval = (uint64_t) self->pAny->val.cr;
            success = true;
         }
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

float dtl_sv_to_flt(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   float retval = 0.0f;

   if (self != NULL)
   {
      switch (dtl_sv_type(self))
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
         retval = (float) (self->pAny->val.bl ? 1.0f : 0.0f);
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (float) self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

double dtl_sv_to_dbl(const dtl_sv_t *self, bool *ok)
{
   bool success = false;
   double retval = 0.0;

   if (self != NULL)
   {
      switch (dtl_sv_type(self))
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
         retval = self->pAny->val.bl ? 1.0 : 0.0;
         success = true;
         break;
      case DTL_SV_CHAR:
         retval = (double) self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   if (ok != NULL)
   {
      *ok = success;
   }
   return retval;
}

char dtl_sv_to_char(const dtl_sv_t *self, bool *ok)
{
   if (self != NULL)
   {
      bool success = false;
      char retval = '\0';
      if (ok != NULL)
      {
         *ok = true;
      }
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         if (ok != NULL)
         {
            *ok = false;
         }
         break;
      case DTL_SV_I32:
         if ((self->pAny->val.i32 >= DTL_CHAR_MIN) && (self->pAny->val.i32 <= DTL_CHAR_MAX))
         {
            retval = (char) self->pAny->val.i32;
            success = true;
         }
         break;
      case DTL_SV_U32:
         if (self->pAny->val.u32 <= (uint32_t) DTL_CHAR_MAX)
         {
            retval = (char) self->pAny->val.u32;
            success = true;
         }
         break;
      case DTL_SV_I64:
         if ((self->pAny->val.i64 >= DTL_CHAR_MIN) && (self->pAny->val.i64 <= DTL_CHAR_MAX))
         {
            retval = (char) self->pAny->val.i64;
            success = true;
         }
         break;
      case DTL_SV_U64:
         if (self->pAny->val.u64 <= (uint64_t) DTL_CHAR_MAX)
         {
            retval = (char) self->pAny->val.u64;
            success = true;
         }
         break;
      case DTL_SV_CHAR:
         retval = self->pAny->val.cr;
         success = true;
         break;
      case DTL_SV_FLT:
         if ((self->pAny->val.flt >= -128.0f) && (self->pAny->val.flt <= 127.0f))
         {
            retval = (char) self->pAny->val.flt;
            success = true;
         }
         break;
      case DTL_SV_DBL:
         if ((self->pAny->val.dbl >= -128.0) && (self->pAny->val.dbl <= 127.0))
         {
            retval = (char) self->pAny->val.dbl;
            success = true;
         }
         break;
      case DTL_SV_BOOL:
         retval = self->pAny->val.bl ? (char) 1 : (char) 0;
         success = true;
         break;
      case DTL_SV_STR:
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         if (ok != NULL)
         {
            *ok = false;
         }
         break;
      }
      if (ok != NULL)
      {
         *ok = success;
      }
      return retval;
   }
   if (ok != NULL)
   {
      *ok = false;
   }
   return '\0';
}

bool dtl_sv_to_bool(const dtl_sv_t *self, bool *ok)
{
   if (self != NULL)
   {
      if (ok != NULL)
      {
         *ok = true;
      }
      bool retval = false;
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         if (ok != NULL)
         {
            *ok = false;
         }
         break;
      case DTL_SV_I32:
         retval = (self->pAny->val.i32 != 0);
         break;
      case DTL_SV_U32:
         retval = (self->pAny->val.u32 != 0u);
         break;
      case DTL_SV_I64:
         retval = (self->pAny->val.i64 != 0);
         break;
      case DTL_SV_U64:
         retval = (self->pAny->val.u64 != 0u);
         break;
      case DTL_SV_FLT:
         retval = (self->pAny->val.flt != 0.0f);
         break;
      case DTL_SV_DBL:
         retval = (self->pAny->val.dbl != 0.0);
         break;
      case DTL_SV_CHAR:
         retval = (self->pAny->val.cr != '\0');
         break;
      case DTL_SV_BOOL:
         retval = self->pAny->val.bl;
         break;
      case DTL_SV_STR:
         if (adt_str_equal_cstr(self->pAny->val.str, "true") || adt_str_equal_cstr(self->pAny->val.str, "TRUE"))
         {
            retval = true;
         }
         else if (!adt_str_equal_cstr(self->pAny->val.str, "false") && !adt_str_equal_cstr(self->pAny->val.str, "FALSE"))
         {
            if (ok != NULL)
            {
               *ok = false;
            }
         }
         break;
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         if (ok != NULL)
         {
            *ok = false;
         }
         break;
      }
      return retval;
   }
   if (ok != NULL)
   {
      *ok = false;
   }
   return false;
}

void *dtl_sv_to_ptr(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
         return (void *) ((intptr_t) self->pAny->val.i32); // NOLINT(performance-no-int-to-ptr)
      case DTL_SV_U32:
         return (void *) ((uintptr_t) self->pAny->val.u32); // NOLINT(performance-no-int-to-ptr)
      case DTL_SV_I64:
      case DTL_SV_U64:
      case DTL_SV_FLT:
      case DTL_SV_DBL:
      case DTL_SV_CHAR:
      case DTL_SV_BOOL:
         break;
      case DTL_SV_STR:
         return (void *) &self->pAny->val.str;
      case DTL_SV_PTR:
         return self->pAny->val.ptr.p;
      case DTL_SV_DV:
         return (void *) self->pAny->val.dv;
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   return NULL;
}

const char *dtl_sv_to_cstr(dtl_sv_t *self, bool *ok)
{
   if (self != NULL)
   {
      if (ok != NULL)
      {
         *ok = false;
      }
      switch (dtl_sv_type(self))
      {
      case DTL_SV_NONE:
         break;
      case DTL_SV_I32:
      case DTL_SV_U32:
      case DTL_SV_I64:
      case DTL_SV_U64:
      case DTL_SV_FLT:
      case DTL_SV_DBL:
      case DTL_SV_CHAR:
         if (self->pAny->tmp_str == NULL)
         {
            self->pAny->tmp_str = adt_str_new();
         }
         else
         {
            adt_str_clear(self->pAny->tmp_str);
         }
         if (self->pAny->tmp_str != NULL)
         {
            dtl_sv_to_string_internal(self, self->pAny->tmp_str, ok);
            return adt_str_cstr(self->pAny->tmp_str);
         }
         break;
      case DTL_SV_BOOL:
         if (ok != NULL)
         {
            *ok = true;
         }
         return self->pAny->val.bl ? "true" : "false";
      case DTL_SV_STR:
         if (ok != NULL)
         {
            *ok = true;
         }
         return adt_str_cstr(self->pAny->val.str);
      case DTL_SV_PTR:
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
         break;
      }
   }
   return NULL;
}

adt_str_t *dtl_sv_to_str(const dtl_sv_t *self, bool *ok)
{
   adt_str_t *str = NULL;
   if (self != NULL)
   {
      str = adt_str_new();
      if (str != NULL)
      {
         dtl_sv_to_string_internal(self, str, ok);
      }
   }
   return str;
}

dtl_dv_t *dtl_sv_to_dv(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      if (dtl_sv_type(self) == DTL_SV_DV)
      {
         return self->pAny->val.dv;
      }
   }
   return NULL;
}

dtl_sv_t *dtl_sv_to_sv(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      if (dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_SCALAR)
         {
            return (dtl_sv_t *) dv;
         }
      }
   }
   return NULL;
}

struct dtl_av_tag *dtl_sv_to_av(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      if (dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_ARRAY)
         {
            return (dtl_av_t *) dv;
         }
      }
   }
   return NULL;
}

struct dtl_hv_tag *dtl_sv_to_hv(const dtl_sv_t *self)
{
   if (self != NULL)
   {
      if (dtl_sv_type(self) == DTL_SV_DV)
      {
         dtl_dv_t *dv = self->pAny->val.dv;
         if (dtl_dv_type(dv) == DTL_DV_HASH)
         {
            return (dtl_hv_t *) dv;
         }
      }
   }
   return NULL;
}

// Comparison functions

dtl_error_t dtl_sv_lt(const dtl_sv_t *self, const dtl_sv_t *other, bool *result)
{
   if ((self != NULL) && (other != NULL) && (result != NULL))
   {
      dtl_error_t retval = DTL_TYPE_ERROR;
      dtl_sv_type_id left_type;
      dtl_sv_type_id right_type;
      *result = false;
      left_type = dtl_sv_type(self);
      right_type = dtl_sv_type(other);
      switch (left_type)
      {
      case DTL_SV_NONE:
         retval = DTL_TYPE_ERROR;
         break;
      case DTL_SV_I32:
         if (right_type == DTL_SV_I32)
         {
            *result = self->pAny->val.i32 < other->pAny->val.i32;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_U32:
         if (right_type == DTL_SV_U32)
         {
            *result = self->pAny->val.u32 < other->pAny->val.u32;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_I64:
         if (right_type == DTL_SV_I64)
         {
            *result = self->pAny->val.i64 < other->pAny->val.i64;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_U64:
         if (right_type == DTL_SV_U64)
         {
            *result = self->pAny->val.u64 < other->pAny->val.u64;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_FLT:
         if (right_type == DTL_SV_FLT)
         {
            *result = self->pAny->val.flt < other->pAny->val.flt;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_DBL:
         if (right_type == DTL_SV_DBL)
         {
            *result = self->pAny->val.dbl < other->pAny->val.dbl;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_BOOL:
         if (right_type == DTL_SV_BOOL)
         {
            *result = self->pAny->val.bl < other->pAny->val.bl;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_CHAR:
         if (right_type == DTL_SV_CHAR)
         {
            *result = self->pAny->val.cr < other->pAny->val.cr;
            retval = DTL_NO_ERROR;
         }
         break;
      case DTL_SV_STR:
         if (right_type == DTL_SV_STR)
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
      case DTL_SV_DV:
      case DTL_SV_BYTES:
      case DTL_SV_BYTEARRAY:
      default:
         retval = DTL_TYPE_ERROR;
         break;
      }
      return retval;
   }
   return DTL_INVALID_ARGUMENT_ERROR;
}

int dtl_sv_vlt(const void *a, const void *b)
{
   if ((a != NULL) && (b != NULL))
   {
      const dtl_dv_t *dv_a = (const dtl_dv_t *) a;
      const dtl_dv_t *dv_b = (const dtl_dv_t *) b;
      if ((dtl_dv_type(dv_a) == DTL_DV_SCALAR) && (dtl_dv_type(dv_b) == DTL_DV_SCALAR))
      {
         bool result = false;
         dtl_error_t err = dtl_sv_lt((const dtl_sv_t *) dv_a, (const dtl_sv_t *) dv_b, &result);
         if (err == DTL_NO_ERROR)
         {
            return result ? 1 : 0;
         }
      }
   }
   return -1;
}

const adt_bytes_t *dtl_sv_get_bytes(const dtl_sv_t *self)
{
   const adt_bytes_t *retval = NULL;
   if (self != NULL)
   {
      dtl_sv_type_id current_type = dtl_sv_type(self);
      if (current_type == DTL_SV_BYTES)
      {
         retval = self->pAny->val.bytes;
      }
   }
   return retval;
}

const adt_bytearray_t *dtl_sv_get_bytearray(const dtl_sv_t *self)
{
   const adt_bytearray_t *retval = NULL;
   if (self != NULL)
   {
      dtl_sv_type_id current_type = dtl_sv_type(self);
      if (current_type == DTL_SV_BYTEARRAY)
      {
         retval = self->pAny->val.bytearray;
      }
   }
   return retval;
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void dtl_sv_set_type(dtl_sv_t *self, dtl_sv_type_id new_type)
{
   dtl_sv_type_id current_type = dtl_sv_type(self);
   if (current_type == DTL_SV_DV)
   {
      dtl_dv_dec_ref(self->pAny->val.dv);
      self->pAny->val.dv = NULL;
   }
   else if (current_type == DTL_SV_PTR)
   {
      if (self->pAny->val.ptr.destructor != NULL)
      {
         self->pAny->val.ptr.destructor(self->pAny->val.ptr.p);
         self->pAny->val.ptr.destructor = NULL;
      }
      self->pAny->val.ptr.p = NULL;
   }
   else if (current_type == DTL_SV_STR)
   {
      if (new_type != DTL_SV_STR)
      {
         adt_str_delete(self->pAny->val.str);
         self->pAny->val.str = NULL;
      }
   }
   else if (current_type == DTL_SV_BYTES)
   {
      adt_bytes_delete(self->pAny->val.bytes);
      self->pAny->val.bytes = NULL;
   }
   else if (current_type == DTL_SV_BYTEARRAY)
   {
      if (new_type != DTL_SV_BYTEARRAY)
      {
         adt_bytearray_delete(self->pAny->val.bytearray);
         self->pAny->val.bytearray = NULL;
      }
   }

   if (new_type == DTL_SV_STR)
   {
      if (current_type != DTL_SV_STR)
      {
         self->pAny->val.str = adt_str_new();
      }
      else
      {
         adt_str_clear(self->pAny->val.str);
      }
   }
   else if (new_type == DTL_SV_BYTEARRAY)
   {
      if (current_type != DTL_SV_BYTEARRAY)
      {
         self->pAny->val.bytearray = adt_bytearray_new();
      }
      else
      {
         adt_bytearray_clear(self->pAny->val.bytearray);
      }
   }

   self->u32Flags &= ~((uint32_t) DTL_SV_TYPE_MASK);
   self->u32Flags |= (((uint32_t) new_type) << DTL_SV_TYPE_SHIFT) & DTL_SV_TYPE_MASK;
}

static void dtl_sv_ztrim(char *str)
{
   char *begin = str;
   char *end = begin + strlen(str);
   char *p = begin;
   char *a = NULL;
   char *b = NULL;
   while (p < end)
   {
      if (!isdigit((unsigned char) *p))
      {
         if (*p == '.')
         {
            a = p;
            break;
         }
         return; // not a number
      }
      ++p;
   }
   p = end - 1;
   while (p > a)
   {
      if (*p == '0')
      {
         b = p;
      }
      else
      {
         break;
      }
      --p;
   }
   if ((a != NULL) && (b != NULL))
   {
      assert(b > a);
      if (b - 1 == a)
      {
         --b; // nothing on the right side of the '.'
      }
      *b = '\0';
   }
   if (strlen(str) == 0)
   {
      str[0] = '0';
      str[1] = '\0';
   }
}

static void dtl_sv_to_string_internal(const dtl_sv_t *self, adt_str_t *str, bool *ok)
{
   char num_buf[MAX_NUM_BUF];
   bool is_num = false;
   switch (dtl_sv_type(self))
   {
   case DTL_SV_NONE:
      if (ok != NULL)
      {
         *ok = true;
      }
      adt_str_append_cstr(str, "(undefined)");
      break;
   case DTL_SV_I32:
   case DTL_SV_CHAR:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%d", (int) dtl_sv_to_i32(self, NULL));
      is_num = true;
      break;
   case DTL_SV_U32:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%u", (unsigned int) dtl_sv_to_u32(self, NULL));
      is_num = true;
      break;
   case DTL_SV_I64:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%lld", (long long int) dtl_sv_to_i64(self, NULL));
      is_num = true;
      break;
   case DTL_SV_U64:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%llu", (long long unsigned int) dtl_sv_to_u64(self, NULL));
      is_num = true;
      break;
   case DTL_SV_FLT:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%f", (double) dtl_sv_to_flt(self, NULL));
      is_num = true;
      dtl_sv_ztrim(num_buf);
      break;
   case DTL_SV_DBL:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%f", dtl_sv_to_dbl(self, NULL));
      is_num = true;
      dtl_sv_ztrim(num_buf);
      break;
   case DTL_SV_BOOL:
      if (ok != NULL)
      {
         *ok = true;
      }
      if (dtl_sv_to_bool(self, NULL))
      {
         adt_str_set_cstr(str, "true");
      }
      else
      {
         adt_str_set_cstr(str, "false");
      }
      break;
   case DTL_SV_STR:
      if (ok != NULL)
      {
         *ok = true;
      }
      adt_str_set(str, self->pAny->val.str);
      break;
   case DTL_SV_PTR:
      if (ok != NULL)
      {
         *ok = true;
      }
      snprintf(num_buf, sizeof(num_buf), "%p", dtl_sv_to_ptr(self));
      is_num = true;
      break;
   case DTL_SV_DV:
   case DTL_SV_BYTES:
   case DTL_SV_BYTEARRAY:
      break;
   }
   if (is_num)
   {
      adt_str_set_cstr(str, num_buf);
   }
}
