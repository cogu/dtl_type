/*****************************************************************************
* \file      dtl_av.c
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
//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dtl_av.h"
#include "dtl_sv.h"
#include <malloc.h>
#include <assert.h>
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static adt_error_t dtl_av_insertion_sort(dtl_av_t *self, bool reverse);


//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
dtl_av_t* dtl_av_new(){
   dtl_av_t *self;
   if((self = (dtl_av_t*)malloc(sizeof(dtl_av_t)))==(dtl_av_t*)0){
      return (dtl_av_t*)0;
   }
   if((self->pAny = (adt_ary_t*)malloc(sizeof(adt_ary_t)))==(adt_ary_t*)0){
      free(self);
      return (dtl_av_t*)0;
   }
   dtl_av_create(self);
   return self;
}

dtl_av_t* dtl_av_make(dtl_dv_t** ppValue, int32_t s32Len){
   if(s32Len<0){
      return (dtl_av_t*)0;
   }
   dtl_av_t *self = dtl_av_new();
   if(self){
      int32_t s32i;
      dtl_av_extend(self,s32Len);
      for(s32i=0;s32i<s32Len;s32i++){
         dtl_av_set(self,s32i,ppValue[s32i]);
      }
   }
   return self;
}

void dtl_av_delete(dtl_av_t *self){
   if(self){
      dtl_av_destroy(self);
      free(self->pAny);
      free(self);
   }
}

void dtl_av_create(dtl_av_t *self){
   if(self){
      adt_ary_create(self->pAny, dtl_dv_dec_ref_void);
      adt_ary_set_fill_elem(self->pAny,(void*) &g_dtl_sv_none);
      self->u32Flags = ((uint32_t)DTL_DV_ARRAY);
      self->u32RefCnt = 1;
   }
}
void dtl_av_destroy(dtl_av_t *self){
   if(self){
      adt_ary_destroy(self->pAny);
   }
}


//Accessors
dtl_dv_t**  dtl_av_set(dtl_av_t *self, int32_t s32Index, dtl_dv_t *pValue){
   if(self){
      dtl_dv_t **tmp = (dtl_dv_t**)adt_ary_get(self->pAny,s32Index);
      if(tmp && *tmp != pValue){
         dtl_dv_dec_ref(*tmp);
      }
      return (dtl_dv_t**) adt_ary_set(self->pAny,s32Index,pValue);
   }
   return (dtl_dv_t**) 0;
}

dtl_dv_t**  dtl_av_get(const dtl_av_t *self, int32_t s32Index){
   if(self){
      return (dtl_dv_t**) adt_ary_get(self->pAny,s32Index);
   }
   return (dtl_dv_t**) 0;
}

dtl_dv_t*  dtl_av_value(const dtl_av_t *self, int32_t s32Index){
   if(self){
      return (dtl_dv_t*) adt_ary_value(self->pAny,s32Index);
   }
   return (dtl_dv_t*) 0;
}

void dtl_av_push(dtl_av_t *self, dtl_dv_t *dv, bool autoIncrementRef){
   if(self){
      adt_ary_push(self->pAny, dv);
      if (autoIncrementRef)
      {
         dtl_dv_inc_ref(dv);
      }
   }
}
dtl_dv_t* dtl_av_pop(dtl_av_t *self){
   if(self){
      return (dtl_dv_t*) adt_ary_pop(self->pAny);
   }
   return (dtl_dv_t*)0;
}

dtl_dv_t*   dtl_av_shift(dtl_av_t *self){
   if(self){
      return (dtl_dv_t*) adt_ary_shift(self->pAny);
   }
   return (dtl_dv_t*)0;
}
void dtl_av_unshift(dtl_av_t *self, dtl_dv_t *pValue){
   if(self){
      adt_ary_unshift(self->pAny,pValue);
   }
}


//Utility functions
void  dtl_av_extend(dtl_av_t *self, int32_t s32Len){
   if(self){
      adt_ary_extend(self->pAny,s32Len);
   }
}
void  dtl_av_fill(dtl_av_t *self, int32_t s32Len){
   if(self){
      adt_ary_fill(self->pAny,s32Len);
   }
}

void  dtl_av_clear(dtl_av_t *self){
   if(self){
      adt_ary_clear(self->pAny);
   }
}

int32_t dtl_av_length(const dtl_av_t *self){
   if(self){
      return adt_ary_length(self->pAny);
   }
   return -1;
}
bool dtl_av_exists(const dtl_av_t *self, int32_t s32Index){
   if(self){
      return adt_ary_exists(self->pAny,s32Index);
   }
   return false;
}

dtl_error_t dtl_av_sort(dtl_av_t *self, dtl_key_func_t *key, bool reverse)
{
   if (self != 0)
   {
      if (key != 0)
      {
         return DTL_NOT_IMPLEMENTED_ERROR;
      }
      return dtl_av_insertion_sort(self, reverse);
   }
   return DTL_INVALID_ARGUMENT_ERROR;
}
//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
static adt_error_t dtl_av_insertion_sort(dtl_av_t *self, bool reverse)
{
   int32_t arrayLen = self->pAny->s32CurLen;
   if (arrayLen > 1)
   {
      int32_t unsortedStart = 1;

      while(unsortedStart < arrayLen)
      {
         bool result = false;
         dtl_error_t errorCode = DTL_NO_ERROR;
         dtl_dv_type_id leftType, rightType;
         dtl_dv_t *left = self->pAny->pFirst[unsortedStart-1];
         dtl_dv_t *right = self->pAny->pFirst[unsortedStart];
         assert( (left != 0) && (right != 0) );
         leftType = dtl_dv_type(left);
         rightType = dtl_dv_type(right);
         if ( (leftType == DTL_DV_SCALAR) && (rightType == DTL_DV_SCALAR) )
         {
            errorCode = dtl_sv_lt( (dtl_sv_t*) left, (dtl_sv_t*) right, &result);
         }
         else
         {
            return DTL_TYPE_ERROR;
         }

         if (errorCode != DTL_NO_ERROR)
         {
            return errorCode;
         }
         else
         {
            if (reverse)
            {
               result = !result;
            }
            if (result == false)
            {
               int32_t i;
               self->pAny->pFirst[unsortedStart] = left;
               for(i=unsortedStart-1; i>0; i--)
               {
                  left = self->pAny->pFirst[i-1];
                  leftType = dtl_dv_type(left);
                  if ( (leftType == DTL_DV_SCALAR) )
                  {
                     errorCode = dtl_sv_lt( (dtl_sv_t*) left, (dtl_sv_t*) right, &result);
                     if (errorCode != DTL_NO_ERROR)
                     {
                        return errorCode;
                     }
                  }
                  else
                  {
                     return DTL_TYPE_ERROR;
                  }
                  if (reverse)
                  {
                     result = !result;
                  }
                  if (result == false)
                  {
                     self->pAny->pFirst[i] = left;
                  }
                  else
                  {
                     break;
                  }
               }
               self->pAny->pFirst[i] = right;
            }
            unsortedStart++;
         }
      }
   }
   return DTL_NO_ERROR;
}

