/*****************************************************************************
* \file      dtl_dv.c
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     Dynamic Value base type implementation
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
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

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void dtl_dv_create(dtl_dv_t *self);

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

dtl_dv_t *dtl_dv_null(void)
{
   dtl_dv_t *self = (dtl_dv_t *) malloc(sizeof(dtl_dv_t));
   if (self == NULL)
   {
      return NULL;
   }
   dtl_dv_create(self);
   return self;
}

void dtl_dv_delete(dtl_dv_t *dv)
{
   if (dv != NULL)
   {
      switch (dtl_dv_type(dv))
      {
      case DTL_DV_INVALID:
         break;
      case DTL_DV_NULL:
         free(dv);
         break;
      case DTL_DV_SCALAR:
         dtl_sv_delete((dtl_sv_t *) dv);
         break;
      case DTL_DV_ARRAY:
         dtl_av_delete((dtl_av_t *) dv);
         break;
      case DTL_DV_HASH:
         dtl_hv_delete((dtl_hv_t *) dv);
         break;
      }
   }
}

void dtl_dv_vdelete(void *arg)
{
   dtl_dv_delete((dtl_dv_t *) arg);
}

void dtl_dv_inc_ref(dtl_dv_t *dv)
{
   if (dv != NULL)
   {
      dv->u32RefCnt++;
   }
}

void dtl_dv_dec_ref(dtl_dv_t *dv)
{
   if ((dv != NULL) && (dv != (dtl_dv_t *) &g_dtl_sv_none) && (dv->u32RefCnt > 0u))
   {
      if (--dv->u32RefCnt == 0u)
      {
         dtl_dv_delete(dv);
      }
   }
}

dtl_dv_type_id dtl_dv_type(const dtl_dv_t *dv)
{
   if (dv == NULL)
   {
      return DTL_DV_INVALID;
   }
   uint8_t type_id = (uint8_t) ((dv->u32Flags & DTL_DV_TYPE_MASK) >> DTL_DV_TYPE_SHIFT);
   return (dtl_dv_type_id) type_id;
}

void dtl_dv_dec_ref_void(void *ptr)
{
   dtl_dv_dec_ref((dtl_dv_t *) ptr);
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void dtl_dv_create(dtl_dv_t *self)
{
   if (self != NULL)
   {
      self->pAny = NULL;
      self->u32RefCnt = 1u;
      self->u32Flags = ((uint32_t) DTL_DV_NULL);
   }
}
