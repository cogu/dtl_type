/*****************************************************************************
* \file:   		dtl_hv.c
* \author:		Conny Gustafsson
* \date:		2013-07-22
* \brief:		DTL hash
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#include <malloc.h>
#include <assert.h>
#include "dtl_hv.h"
#include "dtl_sv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#else
#define vfree free
#endif



/**************** Private Function Declarations *******************/


/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
//Constructor/Destructor
dtl_hv_t* dtl_hv_new(void)
{
	dtl_hv_t *self;
	if( (self = (dtl_hv_t*) malloc(sizeof(dtl_hv_t))) == (dtl_hv_t*) 0 )
	{
		return (dtl_hv_t*) 0;
	}
	if( (self->pAny = (adt_hash_t*) malloc(sizeof(adt_hash_t))) == (adt_hash_t*) 0 )
	{
		free(self);
		return (dtl_hv_t*)0;
	}
	dtl_hv_create(self);
	return self;
}

void dtl_hv_delete(dtl_hv_t *self)
{
	if(self)
	{
		dtl_hv_destroy(self);
		free(self->pAny);
		free(self);
	}
}

void dtl_hv_create(dtl_hv_t *self)
{
	if(self)
	{
		adt_hash_create(self->pAny,dtl_dv_dec_ref_void);
		self->u32Flags = ((uint32_t)DTL_DV_HASH);
		self->u32RefCnt = 1;
	}
}

void dtl_hv_destroy(dtl_hv_t *self)
{
	if(self)
	{
		adt_hash_destroy(self->pAny);
	}
}


//Accessors
void dtl_hv_set_cstr(dtl_hv_t *self, const char *pKey, dtl_dv_t *dv, bool autoIncrementRef)
{
	if(self)
	{
		dtl_dv_t *current = (dtl_dv_t*) adt_hash_get(self->pAny, pKey);
		if( (current != 0) && (current != dv) )
		{
			dtl_dv_dec_ref(current);
		}
		adt_hash_set(self->pAny,pKey,dv);
      if (autoIncrementRef)
      {
         dtl_dv_inc_ref(dv);
      }
	}
}

dtl_dv_t* dtl_hv_get_cstr(const dtl_hv_t *self, const char *pKey)
{
	if(self)
	{
	   void **result = adt_hash_get(self->pAny,pKey);
	   if (result != 0)
	   {
	      return (dtl_dv_t*) *result;
	   }
	}
	return (dtl_dv_t*) 0;
}

dtl_dv_t* dtl_hv_remove_cstr(dtl_hv_t *self, const char *pKey)
{
	if(self)
	{
		return (dtl_dv_t*) adt_hash_remove(self->pAny,pKey);
	}
	return (dtl_dv_t*) 0;
}

void dtl_hv_iter_init(dtl_hv_t *self)
{
	if(self)
	{
		adt_hash_iter_init(self->pAny);
	}
}

/**
 * \param[in] self hash
 * \param[out] pKey pointer to key string
 * \return pointer to scalar value
 */
dtl_dv_t* dtl_hv_iter_next_cstr(dtl_hv_t *self, const char **ppKey)
{
	if(self)
	{
		return (dtl_dv_t*) adt_hash_iter_next(self->pAny, ppKey);
	}
	return (dtl_dv_t*) 0;
}



//Utility functions
uint32_t dtl_hv_length(const dtl_hv_t *self)
{
	if(self)
	{
		return adt_hash_length(self->pAny);
	}
	return (uint32_t) 0;
}

bool dtl_hv_exists_cstr(const dtl_hv_t *self, const char *pKey)
{
	if(self)
	{
		return adt_hash_exists(self->pAny,pKey);
	}
	return false;
}

/**
 * Returns new DTL Array containing the keys found in the hash.
 * Each item in the returned array is of type dtl_sv_t (where scalar type is string).
 * The caller is responsible of disposing the array after use (preferably by calling dtl_dec_ref).
 */
dtl_av_t* dtl_hv_keys(const dtl_hv_t *self)
{
	if( (self != 0) )
	{
	   dtl_av_t *array = 0;
	   adt_ary_t *tmp = 0; //adt_ary returns a list of cstr (C strings).
	   array = dtl_av_new();
	   if (array != 0)
	   {
	      tmp = adt_ary_new(vfree);
	   }
	   if ( (array != 0) && (tmp != 0) )
	   {
	      int32_t numItems = adt_hash_keys(self->pAny, tmp);
	      if (numItems > 0)
	      {
	         int32_t i;
	         dtl_av_clear(array);
	         for (i=0; i<numItems; i++)
	         {
	            const char *key = (const char *) adt_ary_value(tmp, i);
	            assert(key != 0);
	            dtl_av_push(array, (dtl_dv_t*) dtl_sv_make_cstr(key), false);
	         }
	      }
	      assert(dtl_av_length(array) == numItems);
	   }
	   if (tmp != 0)
	   {
	      adt_ary_delete(tmp);
	   }
	   return array;
	}
	return (dtl_av_t*) 0;
}

/***************** Private Function Definitions *******************/

