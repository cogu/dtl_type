/*****************************************************************************
* \file      dtl_av.c
* \author    Conny Gustafsson
* \date      2013-03-10
* \brief     DTL Array Value implementation
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
#include "dtl_av.h"
#include "dtl_sv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

// Constructor/Destructor

dtl_av_t *dtl_av_new(void)
{
   dtl_av_t *self = (dtl_av_t *) malloc(sizeof(dtl_av_t));
   if (self == NULL)
   {
      return NULL;
   }
   self->pAny = (adt_ary_t *) malloc(sizeof(adt_ary_t));
   if (self->pAny == NULL)
   {
      free(self);
      return NULL;
   }
   dtl_av_create(self);
   return self;
}

dtl_av_t *dtl_av_make(dtl_dv_t **values, int32_t len)
{
   if (len < 0)
   {
      return NULL;
   }
   dtl_av_t *self = dtl_av_new();
   if ((self != NULL) && (values != NULL))
   {
      int32_t i;
      dtl_av_extend(self, len);
      for (i = 0; i < len; i++)
      {
         dtl_av_set(self, i, values[i]);
      }
   }
   return self;
}

void dtl_av_delete(dtl_av_t *self)
{
   if (self != NULL)
   {
      dtl_av_destroy(self);
      free(self->pAny);
      free(self);
   }
}

void dtl_av_create(dtl_av_t *self)
{
   if (self != NULL)
   {
      adt_ary_create(self->pAny, dtl_dv_dec_ref_void);
      adt_ary_set_fill_elem(self->pAny, (void *) &g_dtl_sv_none);
      self->u32Flags = ((uint32_t) DTL_DV_ARRAY);
      self->u32RefCnt = 1u;
   }
}

void dtl_av_destroy(dtl_av_t *self)
{
   if (self != NULL)
   {
      adt_ary_destroy(self->pAny);
   }
}

// Accessors

dtl_dv_t **dtl_av_set(dtl_av_t *self, int32_t index, dtl_dv_t *value)
{
   if (self != NULL)
   {
      dtl_dv_t **tmp = (dtl_dv_t **) adt_ary_get(self->pAny, index);
      if ((tmp != NULL) && (*tmp != value))
      {
         dtl_dv_dec_ref(*tmp);
      }
      return (dtl_dv_t **) adt_ary_set(self->pAny, index, value);
   }
   return NULL;
}

dtl_dv_t **dtl_av_get(const dtl_av_t *self, int32_t index)
{
   if (self != NULL)
   {
      return (dtl_dv_t **) adt_ary_get(self->pAny, index);
   }
   return NULL;
}

dtl_dv_t *dtl_av_value(const dtl_av_t *self, int32_t index)
{
   if (self != NULL)
   {
      return (dtl_dv_t *) adt_ary_value(self->pAny, index);
   }
   return NULL;
}

void dtl_av_push(dtl_av_t *self, dtl_dv_t *dv, bool auto_increment_ref)
{
   if (self != NULL)
   {
      adt_ary_push(self->pAny, dv);
      if (auto_increment_ref)
      {
         dtl_dv_inc_ref(dv);
      }
   }
}

dtl_dv_t *dtl_av_pop(dtl_av_t *self)
{
   if (self != NULL)
   {
      return (dtl_dv_t *) adt_ary_pop(self->pAny);
   }
   return NULL;
}

dtl_dv_t *dtl_av_shift(dtl_av_t *self)
{
   if (self != NULL)
   {
      return (dtl_dv_t *) adt_ary_shift(self->pAny);
   }
   return NULL;
}

void dtl_av_unshift(dtl_av_t *self, dtl_dv_t *value)
{
   if (self != NULL)
   {
      adt_ary_unshift(self->pAny, value);
   }
}

// Utility functions

void dtl_av_extend(dtl_av_t *self, int32_t len)
{
   if (self != NULL)
   {
      int32_t old_len = adt_ary_length(self->pAny);
      adt_ary_extend(self->pAny, len);
      int32_t new_len = adt_ary_length(self->pAny);
      int32_t i;
      for (i = old_len; i < new_len; i++)
      {
         self->pAny->pFirst[i] = (void *) &g_dtl_sv_none;
      }
   }
}

void dtl_av_fill(dtl_av_t *self, int32_t len)
{
   if (self != NULL)
   {
      adt_ary_fill(self->pAny, len);
   }
}

void dtl_av_clear(dtl_av_t *self)
{
   if (self != NULL)
   {
      adt_ary_clear(self->pAny);
   }
}

int32_t dtl_av_length(const dtl_av_t *self)
{
   if (self != NULL)
   {
      return adt_ary_length(self->pAny);
   }
   return -1;
}

bool dtl_av_is_empty(const dtl_av_t *self)
{
   if (self != NULL)
   {
      return adt_ary_is_empty(self->pAny);
   }
   return false;
}

bool dtl_av_exists(const dtl_av_t *self, int32_t index)
{
   if (self != NULL)
   {
      return adt_ary_exists(self->pAny, index) > 0;
   }
   return false;
}

dtl_error_t dtl_av_sort(dtl_av_t *self, dtl_key_func_t *key, bool reverse)
{
   if (self != NULL)
   {
      if (key != NULL)
      {
         return DTL_NOT_IMPLEMENTED_ERROR;
      }
      adt_error_t err = adt_ary_sort(self->pAny, dtl_sv_vlt, reverse);
      if (err == ADT_NO_ERROR)
      {
         return DTL_NO_ERROR;
      }
      if (err == ADT_OBJECT_COMPARE_ERROR)
      {
         return DTL_TYPE_ERROR;
      }
      return DTL_INVALID_ARGUMENT_ERROR;
   }
   return DTL_INVALID_ARGUMENT_ERROR;
}
