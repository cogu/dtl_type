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
		adt_ary_create(self->pAny,dtl_dv_ref_dec_void);
		adt_ary_set_fill_elem(self->pAny,(void*) &g_dtl_sv_undef);
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
dtl_dv_t**	dtl_av_set(dtl_av_t *self, int32_t s32Index, dtl_dv_t *pValue){
	if(self){
		dtl_dv_t **tmp = (dtl_dv_t**)adt_ary_get(self->pAny,s32Index);
		if(tmp && *tmp != pValue){
			dtl_dv_ref_dec(*tmp);
		}
		return (dtl_dv_t**) adt_ary_set(self->pAny,s32Index,pValue);
	}
	return (dtl_dv_t**) 0;
}

dtl_dv_t**	dtl_av_get(const dtl_av_t *self, int32_t s32Index){
	if(self){
		return (dtl_dv_t**) adt_ary_get(self->pAny,s32Index);
	}
	return (dtl_dv_t**) 0;
}

void dtl_av_push(dtl_av_t *self, dtl_dv_t *pValue){
	if(self){
		adt_ary_push(self->pAny,pValue);
	}
}
dtl_dv_t* dtl_av_pop(dtl_av_t *self){
	if(self){
		return (dtl_dv_t*) adt_ary_pop(self->pAny);
	}
	return (dtl_dv_t*)0;
}

dtl_dv_t*	dtl_av_shift(dtl_av_t *self){
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
void	dtl_av_extend(dtl_av_t *self, int32_t s32Len){
	if(self){
		adt_ary_extend(self->pAny,s32Len);
	}
}
void	dtl_av_fill(dtl_av_t *self, int32_t s32Len){
	if(self){
		adt_ary_fill(self->pAny,s32Len);
	}
}

void	dtl_av_clear(dtl_av_t *self){
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

/***************** Private Function Definitions *******************/

