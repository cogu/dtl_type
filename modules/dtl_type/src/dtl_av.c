/*****************************************************************************
* \file:   		dtl_av.c
* \author:		Conny Gustafsson
* \date:		2013-07-23
* \brief:		DTL array
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#include "dtl_av.h"
#include "dtl_sv.h"
#include <malloc.h>
#include <assert.h>
#include "CMemLeak.h"

/**************** Private Function Declarations *******************/


/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
//Constructor/Destructor
dtl_av_t* dtl_av_new(){
	dtl_av_t *this;
	if((this = (dtl_av_t*)malloc(sizeof(dtl_av_t)))==(dtl_av_t*)0){
		return (dtl_av_t*)0;
	}
	if((this->pAny = (adt_array_t*)malloc(sizeof(adt_array_t)))==(adt_array_t*)0){
		free(this);
		return (dtl_av_t*)0;
	}
	dtl_av_create(this);
	return this;
}

dtl_av_t* dtl_av_make(dtl_dv_t** ppValue, int32_t s32Len){
	if(s32Len<0){
		return (dtl_av_t*)0;
	}
	dtl_av_t *this = dtl_av_new();
	if(this){
		int32_t s32i;
		dtl_av_extend(this,s32Len);
		for(s32i=0;s32i<s32Len;s32i++){
			dtl_av_set(this,s32i,ppValue[s32i]);
		}
	}
	return this;
}

void		dtl_av_delete(dtl_av_t *this){
	if(this){
		dtl_av_destroy(this);
		free(this->pAny);
		free(this);
	}
}

void dtl_av_create(dtl_av_t *this){
	if(this){
		adt_array_create(this->pAny,dtl_dv_ref_dec_void);
		adt_array_set_fill_elem(this->pAny,(void*) &g_dtl_sv_undef);
		this->u32Flags = ((uint32_t)DTL_DV_ARRAY);
		this->u32RefCnt = 1;
	}
}
void dtl_av_destroy(dtl_av_t *this){
	if(this){
		adt_array_destroy(this->pAny);
	}
}


//Accessors
dtl_dv_t**	dtl_av_set(dtl_av_t *this, int32_t s32Index, dtl_dv_t *pValue){
	if(this){
		dtl_dv_t **tmp = (dtl_dv_t**)adt_array_get(this->pAny,s32Index);
		if(tmp && *tmp != pValue){
			dtl_dv_ref_dec(*tmp);
		}
		return (dtl_dv_t**) adt_array_set(this->pAny,s32Index,pValue);
	}
	return (dtl_dv_t**) 0;
}

dtl_dv_t**	dtl_av_get(dtl_av_t *this, int32_t s32Index){
	if(this){
		return (dtl_dv_t**) adt_array_get(this->pAny,s32Index);
	}
	return (dtl_dv_t**) 0;
}

void dtl_av_push(dtl_av_t *this, dtl_dv_t *pValue){
	if(this){
		adt_array_push(this->pAny,pValue);
	}
}
dtl_dv_t* dtl_av_pop(dtl_av_t *this){
	if(this){
		return (dtl_dv_t*) adt_array_pop(this->pAny);
	}
	return (dtl_dv_t*)0;
}

dtl_dv_t*	dtl_av_shift(dtl_av_t *this){
	if(this){
		return (dtl_dv_t*) adt_array_shift(this->pAny);
	}
	return (dtl_dv_t*)0;
}
void dtl_av_unshift(dtl_av_t *this, dtl_dv_t *pValue){
	if(this){
		adt_array_unshift(this->pAny,pValue);
	}
}


//Utility functions
void	dtl_av_extend(dtl_av_t *this, int32_t s32Len){
	if(this){
		adt_array_extend(this->pAny,s32Len);
	}
}
void	dtl_av_fill(dtl_av_t *this, int32_t s32Len){
	if(this){
		adt_array_fill(this->pAny,s32Len);
	}
}

void	dtl_av_clear(dtl_av_t *this){
	if(this){
		adt_array_clear(this->pAny);
	}
}

int32_t dtl_av_len(dtl_av_t *this){
	if(this){
		return adt_array_length(this->pAny);
	}
	return -1;
}
bool dtl_av_exists(dtl_av_t *this, int32_t s32Index){
	if(this){
		return adt_array_exists(this->pAny,s32Index);
	}
	return false;
}

/***************** Private Function Definitions *******************/

