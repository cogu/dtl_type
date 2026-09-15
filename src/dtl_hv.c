/*****************************************************************************
* \file      dtl_hv.c
* \author    Conny Gustafsson
* \date      2013-07-22
* \brief     DTL Hash Value implementation
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
#include "dtl_hv.h"
#include "dtl_sv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#else
#define vfree free
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

dtl_hv_t *dtl_hv_new(void)
{
   dtl_hv_t *self = (dtl_hv_t *) malloc(sizeof(dtl_hv_t));
   if (self == NULL)
   {
      return NULL;
   }
   self->pAny = (adt_hash_t *) malloc(sizeof(adt_hash_t));
   if (self->pAny == NULL)
   {
      free(self);
      return NULL;
   }
   dtl_hv_create(self);
   return self;
}

void dtl_hv_delete(dtl_hv_t *self)
{
   if (self != NULL)
   {
      dtl_hv_destroy(self);
      free(self->pAny);
      free(self);
   }
}

void dtl_hv_create(dtl_hv_t *self)
{
   if (self != NULL)
   {
      adt_hash_create(self->pAny, dtl_dv_dec_ref_void);
      self->u32Flags = ((uint32_t) DTL_DV_HASH);
      self->u32RefCnt = 1u;
   }
}

void dtl_hv_destroy(dtl_hv_t *self)
{
   if (self != NULL)
   {
      adt_hash_destroy(self->pAny);
   }
}

// Accessors

void dtl_hv_set_cstr(dtl_hv_t *self, const char *key, dtl_dv_t *dv, bool auto_increment_ref)
{
   if (self != NULL)
   {
      dtl_dv_t *current = (dtl_dv_t *) adt_hash_get(self->pAny, key);
      if ((current != NULL) && (current != dv))
      {
         dtl_dv_dec_ref(current);
      }
      adt_hash_set(self->pAny, key, dv);
      if (auto_increment_ref)
      {
         dtl_dv_inc_ref(dv);
      }
   }
}

dtl_dv_t *dtl_hv_get_cstr(const dtl_hv_t *self, const char *key)
{
   if (self != NULL)
   {
      void **result = adt_hash_get(self->pAny, key);
      if (result != NULL)
      {
         return (dtl_dv_t *) *result;
      }
   }
   return NULL;
}

dtl_dv_t *dtl_hv_remove_cstr(dtl_hv_t *self, const char *key)
{
   if (self != NULL)
   {
      return (dtl_dv_t *) adt_hash_remove(self->pAny, key);
   }
   return NULL;
}

void dtl_hv_iter_init(dtl_hv_t *self)
{
   if (self != NULL)
   {
      adt_hash_iter_init(self->pAny);
   }
}

dtl_dv_t *dtl_hv_iter_next_cstr(dtl_hv_t *self, const char **key)
{
   if (self != NULL)
   {
      void **pp_value = adt_hash_iter_next(self->pAny, key);
      if (pp_value != NULL)
      {
         return (dtl_dv_t *) *pp_value;
      }
   }
   return NULL;
}

// Utility functions

uint32_t dtl_hv_length(const dtl_hv_t *self)
{
   if (self != NULL)
   {
      return (uint32_t) adt_hash_length(self->pAny);
   }
   return 0u;
}

bool dtl_hv_exists_cstr(const dtl_hv_t *self, const char *key)
{
   if (self != NULL)
   {
      return adt_hash_exists(self->pAny, key);
   }
   return false;
}

dtl_av_t *dtl_hv_keys(const dtl_hv_t *self)
{
   if (self != NULL)
   {
      dtl_av_t *array = dtl_av_new();
      adt_ary_t *tmp = NULL;
      if (array != NULL)
      {
         tmp = adt_ary_new(NULL);
      }
      if ((array != NULL) && (tmp != NULL))
      {
         int32_t num_items = adt_hash_keys(self->pAny, tmp);
         if (num_items > 0)
         {
            int32_t i;
            dtl_av_clear(array);
            for (i = 0; i < num_items; i++)
            {
               const char *key = (const char *) adt_ary_value(tmp, i);
               assert(key != NULL);
               dtl_av_push(array, (dtl_dv_t *) dtl_sv_make_cstr(key), false);
            }
         }
         assert(dtl_av_length(array) == num_items);
      }
      if (tmp != NULL)
      {
         adt_ary_delete(tmp);
      }
      return array;
   }
   return NULL;
}
