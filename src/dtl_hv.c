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
#include "CMemLeak.h"


/**************** Private Function Declarations *******************/


/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
//Constructor/Destructor
dtl_hv_t* dtl_hv_new(void){
	dtl_hv_t *this;
	if((this = (dtl_hv_t*)malloc(sizeof(dtl_hv_t)))==(dtl_hv_t*)0){
		return (dtl_hv_t*)0;
	}
	if((this->pAny = (adt_hash_t*)malloc(sizeof(adt_hash_t)))==(adt_hash_t*)0){
		free(this);
		return (dtl_hv_t*)0;
	}
	dtl_hv_create(this);
	return this;
}

void dtl_hv_delete(dtl_hv_t *this){
	if(this){
		dtl_hv_destroy(this);
		free(this->pAny);
		free(this);
	}
}

void dtl_hv_create(dtl_hv_t *this){
	if(this){
		adt_hash_create(this->pAny,dtl_dv_ref_dec_void);
		this->u32Flags = ((uint32_t)DTL_DV_HASH);
		this->u32RefCnt = 1;
	}
}

void dtl_hv_destroy(dtl_hv_t *this){
	if(this){
		adt_hash_destroy(this->pAny);
	}
}


//Accessors
void dtl_hv_set(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen, dtl_dv_t *pValue){
	if(this){
		dtl_dv_t *pOld = (dtl_dv_t*) adt_hash_get(this->pAny,pKey,u32KeyLen);
		if(pOld && pOld != pValue){
			dtl_dv_ref_dec(pOld);
		}
		adt_hash_set(this->pAny,pKey,u32KeyLen,pValue);
	}
}

dtl_dv_t* dtl_hv_get(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen){
	if(this){
	   void **result = adt_hash_get(this->pAny,pKey,u32KeyLen);
	   if (result != 0)
	   {
	      return (dtl_dv_t*) *result;
	   }
	}
	return (dtl_dv_t*) 0;
}

dtl_dv_t* dtl_hv_remove(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen){
	if(this){
		return (dtl_dv_t*) adt_hash_remove(this->pAny,pKey,u32KeyLen);
	}
	return (dtl_dv_t*) 0;
}

void dtl_hv_iter_init(dtl_hv_t *this){
	if(this){
		adt_hash_iter_init(this->pAny);
	}
}

/**
 * \param[in] this hash
 * \param[out] pKey pointer to key string
 * \param[out] pKeyLen pointer to integer containing length of key string
 * \return pointer to scalar value
 */
dtl_dv_t* dtl_hv_iter_next(dtl_hv_t *this,const char **ppKey,uint32_t *pKeyLen){
	if(this){
		return (dtl_dv_t*) adt_hash_iter_next(this->pAny,ppKey,pKeyLen);
	}
	return (dtl_dv_t*) 0;
}



//Utility functions
uint32_t dtl_hv_size(dtl_hv_t *this){
	if(this){
		return adt_hash_size(this->pAny);
	}
	return (uint32_t) 0;
}

bool dtl_hv_exists(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen){
	if(this){
		return adt_hash_exists(this->pAny,pKey,u32KeyLen);
	}
	return false;
}

uint32_t dtl_hv_keys(dtl_hv_t *this,dtl_av_t *pArray){
	if(this && pArray){
		return adt_hash_keys(this->pAny,pArray->pAny);
	}
	return 0;
}


/***************** Private Function Definitions *******************/

