/*****************************************************************************
* \file      dtl_sv.c
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

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
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
//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void dtl_sv_set_type(dtl_sv_t *self,dtl_sv_type_id type);
static void dtl_sv_ztrim(char *str);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
static dtl_svx_t g_dtl_svx_undef = {0};
dtl_sv_t g_dtl_sv_undef = {&g_dtl_svx_undef,1,((uint32_t)DTL_DV_SCALAR)};

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void){
   dtl_sv_t *self = (dtl_sv_t*) malloc(sizeof(dtl_sv_t));
   if(self !=(dtl_sv_t*)0){
      dtl_sv_create(self);
   }
   return self;
}

void dtl_sv_delete(dtl_sv_t* self){
   if(self){
      dtl_sv_destroy(self);
      free(self);
   }
}

void dtl_sv_create(dtl_sv_t* self){
   if(self){
      self->pAny = (dtl_svx_t*) malloc(sizeof(dtl_svx_t));
      if (self->pAny != 0)
      {
         memset(self->pAny, 0, sizeof(dtl_svx_t));
         self->u32Flags = ((uint32_t)DTL_DV_SCALAR);
         self->u32RefCnt = 1;
      }
      else
      {
         assert(0);
      }
   }
}

void dtl_sv_destroy(dtl_sv_t* self){
   if(self){
      switch(dtl_sv_type(self)){
      case DTL_SV_STR:
         adt_str_destroy(&self->pAny->str);
         break;
      case DTL_SV_PTR:
         if(self->pAny->ptr.pDestructor){
            self->pAny->ptr.pDestructor(self->pAny->ptr.p);
         }
         break;
      case DTL_SV_DV:
         if(self->pAny->dv){
            dtl_dv_ref_dec(self->pAny->dv);
         }
         break;
      default:
         break;
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

dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*)){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_ptr(self,ptr,pDestructor);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_cstr(const char* str){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_cstr(self,str);
   }
   return self;
}

dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv){
   dtl_sv_t *self = dtl_sv_new();
   if(self){
      dtl_sv_set_dv(self,dv);
   }
   return self;
}

dtl_sv_type_id dtl_sv_type(const dtl_sv_t* self){
   if(self){
      uint8_t u8Type = (uint8_t) ((self->u32Flags & DTL_SV_TYPE_MASK)>>DTL_SV_TYPE_SHIFT);
      return (dtl_sv_type_id) u8Type;
   }
   return DTL_SV_UNDEF;
}

dtl_dv_type_id dtl_sv_dv_type(const dtl_sv_t* self){
   if( (self != 0) && ( (dtl_sv_type(self) == DTL_SV_DV)) ){
      return dtl_dv_type(self->pAny->dv);
   }
   return DTL_DV_INVALID;
}


//Setters
void dtl_sv_set_i32(dtl_sv_t *self, int32_t i32){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_I32);
      self->pAny->i32 = i32;
   }
}

void dtl_sv_set_u32(dtl_sv_t *self, uint32_t u32){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_U32);
      self->pAny->u32 = u32;
   }
}

void dtl_sv_set_i64(dtl_sv_t *self, int64_t i64){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_I64);
      self->pAny->i64 = i64;
   }
}

void dtl_sv_set_u64(dtl_sv_t *self, uint64_t u64){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_U64);
      self->pAny->u64 = u64;
   }
}


void dtl_sv_set_flt(dtl_sv_t *self, float flt){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_FLT);
      self->pAny->flt = flt;
   }
}
void dtl_sv_set_dbl(dtl_sv_t *self, double dbl){
   if(self){
      dtl_sv_set_type(self,DTL_SV_DBL);
      self->pAny->dbl = dbl;
   }
}

void dtl_sv_set_bool(dtl_sv_t *self, bool bl){
   if(self){
      dtl_sv_set_type(self,DTL_SV_BOOL);
      self->pAny->bl = bl;
   }
}

void dtl_sv_set_ptr(dtl_sv_t *self, void *p, void (*pDestructor)(void*)){
   if(self){
      if(dtl_sv_type(self)==DTL_SV_DV){
         dtl_dv_ref_dec(self->pAny->dv);
      }
      dtl_sv_set_type(self,DTL_SV_PTR);
      self->pAny->ptr.p = p;
      self->pAny->ptr.pDestructor = pDestructor;
   }
}

void dtl_sv_set_str(dtl_sv_t *self, const adt_str_t *str)
{
   if (self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set(&self->pAny->str, str);
   }
}

void dtl_sv_set_cstr(dtl_sv_t *self, const char* cstr){
   if(self != 0)
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_cstr(&self->pAny->str, cstr);
   }
}

void dtl_sv_set_bstr(dtl_sv_t *self, const uint8_t *pBegin, const uint8_t *pEnd)
{
   if ( (self != 0) && (pBegin != 0) && (pEnd != 0) && (pBegin<=pEnd) )
   {
      dtl_sv_set_type(self, DTL_SV_STR);
      adt_str_set_bstr(&self->pAny->str, pBegin, pEnd);
   }
}

void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv)
{
   if(self != 0)
   {
      dtl_sv_set_type(self,DTL_SV_DV);
      self->pAny->dv = dv;
      dtl_dv_ref_inc(dv);
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (int32_t) self->pAny->u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (int32_t) self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (int32_t) self->pAny->u64;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = (int32_t) self->pAny->flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (int32_t) self->pAny->dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (int32_t) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = (uint32_t) self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval =  self->pAny->u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (uint32_t) self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (uint32_t) self->pAny->u64;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = (uint32_t) self->pAny->flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (uint32_t) self->pAny->dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (uint32_t) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = (int64_t) self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (int64_t) self->pAny->u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (int64_t) self->pAny->u64;
         success = true;
         break;
     case DTL_SV_FLT:
         retval = (int64_t) self->pAny->flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (int64_t) self->pAny->dbl;
         success = true;
         break;
     case DTL_SV_BOOL:
         retval = (int64_t) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = (uint64_t) self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (uint64_t) self->pAny->u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (uint64_t) self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = self->pAny->u64;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = (uint64_t) self->pAny->flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (uint64_t) self->pAny->dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (uint64_t) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = (float) self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (float) self->pAny->u32;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (float) self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (float) self->pAny->u64;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = self->pAny->flt;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = (float) self->pAny->dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (float) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
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
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         retval = (double) self->pAny->i32;
         success = true;
         break;
      case DTL_SV_U32:
         retval = (double) self->pAny->u32;
         success = true;
         break;
      case DTL_SV_FLT:
         retval = (double) self->pAny->flt;
         success = true;
         break;
      case DTL_SV_I64:
         retval = (double) self->pAny->i64;
         success = true;
         break;
      case DTL_SV_U64:
         retval = (double) self->pAny->u64;
         success = true;
         break;
      case DTL_SV_DBL:
         retval = self->pAny->dbl;
         success = true;
         break;
      case DTL_SV_BOOL:
         retval = (double) self->pAny->bl;
         success = true;
         break;
      case DTL_SV_STR:
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      }
   }
   if (ok != 0)
   {
      *ok = success;
   }
   return retval;
}

bool dtl_sv_to_bool(const dtl_sv_t *self){
   if(self){
      switch(dtl_sv_type(self)){
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         return (bool) self->pAny->i32;
      case DTL_SV_U32:
         return (bool) self->pAny->u32;
      case DTL_SV_I64:
         return (bool) self->pAny->i64;
      case DTL_SV_U64:
         return (bool) self->pAny->u64;
      case DTL_SV_FLT:
         return (bool) self->pAny->flt;
      case DTL_SV_DBL:
         return (bool) self->pAny->dbl;
      case DTL_SV_BOOL:
         return self->pAny->bl;
      case DTL_SV_STR:
         if (adt_str_equal_cstr(&self->pAny->str, "true") ) return true;
         break;
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      }
   }
   return false;
}

void* dtl_sv_to_ptr(const dtl_sv_t *self){
   if(self){
      switch(dtl_sv_type(self)){
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         return (void*) ((long) self->pAny->i32);
      case DTL_SV_U32:
         return (void*) ((long) self->pAny->u32);
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
         return (void*) &self->pAny->str;
         break;
      case DTL_SV_PTR:
         return self->pAny->ptr.p;
         break;
      case DTL_SV_DV:
         return (void*) self->pAny->dv;
         break;
      }
   }
   return (void*)0;
}

const char* dtl_sv_to_cstr(dtl_sv_t *self){
   if(self){
      switch(dtl_sv_type(self)){
      case DTL_SV_UNDEF:
         break;
      case DTL_SV_I32:
         break;
      case DTL_SV_U32:
         break;
      case DTL_SV_I64:
         break;
      case DTL_SV_U64:
         break;
      case DTL_SV_FLT:
         break;
      case DTL_SV_DBL:
         break;
      case DTL_SV_BOOL:
         return self->pAny->bl? "true" : "false";
      case DTL_SV_STR:
         return adt_str_cstr(&self->pAny->str);
      case DTL_SV_PTR:
         break;
      case DTL_SV_DV:
         break;
      }
   }
   return (const char*)0;
}

adt_str_t* dtl_sv_to_string(const dtl_sv_t *self)
{
   adt_str_t *str = (adt_str_t*) 0;
   if(self != 0)
   {
      str = adt_str_new();
      if (str != 0)
      {
         char numBuf[MAX_NUM_BUF];
         bool isNum = false;
         switch(dtl_sv_type(self)){
         case DTL_SV_UNDEF:
            adt_str_append_cstr(str,"(undefined)");
            break;
         case DTL_SV_I32:
            sprintf(numBuf,"%d", (int) dtl_sv_to_i32(self, NULL));
            isNum = true;
            break;
         case DTL_SV_U32:
            sprintf(numBuf,"%u", (unsigned int) dtl_sv_to_u32(self, NULL));
            isNum = true;
            break;
         case DTL_SV_I64:
            sprintf(numBuf,"%lld", (long long int) dtl_sv_to_i64(self, NULL));
            isNum = true;
            break;
         case DTL_SV_U64:
            sprintf(numBuf,"%llu", (long long unsigned int) dtl_sv_to_u64(self, NULL));
            isNum = true;
            break;
         case DTL_SV_FLT:
            sprintf(numBuf,"%f", (double) dtl_sv_to_flt(self, NULL));
            isNum = true;
            dtl_sv_ztrim(numBuf);
            break;
         case DTL_SV_DBL:
            sprintf(numBuf,"%f", dtl_sv_to_dbl(self, NULL));
            isNum = true;
            dtl_sv_ztrim(numBuf);
            break;
         case DTL_SV_BOOL:
            if(dtl_sv_to_bool(self))
            {
               adt_str_set_cstr(str, "true");
            }
            else
            {
               adt_str_set_cstr(str, "false");
            }
            break;
         case DTL_SV_STR:
            adt_str_set(str, &self->pAny->str);
            break;
         case DTL_SV_PTR:
            sprintf(numBuf,"%p", dtl_sv_to_ptr(self));
            isNum = true;
            break;
         case DTL_SV_DV:
            break;
         }
         if(isNum)
         {
            adt_str_set_cstr(str, numBuf);
         }
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
         return self->pAny->dv;
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
         dtl_dv_t *dv = self->pAny->dv;
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
         dtl_dv_t *dv = self->pAny->dv;
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
         dtl_dv_t *dv = self->pAny->dv;
         if (dtl_dv_type(dv) == DTL_DV_HASH )
         {
            return (dtl_hv_t*) dv;
         }
      }
   }
   return (dtl_hv_t*) 0;
}


//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
static void dtl_sv_set_type(dtl_sv_t *self, dtl_sv_type_id newType)
{
   dtl_sv_type_id currentType = dtl_sv_type(self);
   if(currentType == DTL_SV_DV)
   {
      dtl_dv_ref_dec(self->pAny->dv);
   }
   self->u32Flags &= ~((uint32_t)DTL_SV_TYPE_MASK);
   self->u32Flags |= (((uint32_t)newType)<<DTL_SV_TYPE_SHIFT) & DTL_SV_TYPE_MASK;
   if ( (currentType == DTL_SV_STR) && (newType != DTL_SV_STR) )
   {
      adt_str_destroy(&self->pAny->str);
   }
   if ( (currentType != DTL_SV_STR) && (newType == DTL_SV_STR) )
   {
      adt_str_create(&self->pAny->str);
   }
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
