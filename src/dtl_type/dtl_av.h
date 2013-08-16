/*****************************************************************************
* \file:   		dtl_av.h
* \author:		Conny Gustafsson
* \date:		2013-03-10
* \brief:		DTL Array
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#ifndef DTL_AV_H__
#define DTL_AV_H__
#include <stdbool.h>
#include "dtl_dv.h"
#include "adt_array.h"

typedef struct dtl_av_tag{
  DTL_DV_HEAD(adt_array_t);
} dtl_av_t;

/***************** Public Function Declarations *******************/
//Constructor/Destructor
dtl_av_t*	dtl_av_new();
dtl_av_t*	dtl_av_make(dtl_dv_t** ppValue, int32_t s32Len);
void		dtl_av_delete(dtl_av_t *this);
void		dtl_av_create(dtl_av_t *this);
void		dtl_av_destroy(dtl_av_t *this);

//Accessors
dtl_dv_t**	dtl_av_set(dtl_av_t *this, int32_t s32Index, dtl_dv_t *pValue);
dtl_dv_t**	dtl_av_get(dtl_av_t *this, int32_t s32Index);
void		dtl_av_push(dtl_av_t *this, dtl_dv_t *pValue);
dtl_dv_t*	dtl_av_pop(dtl_av_t *this);
dtl_dv_t*	dtl_av_shift(dtl_av_t *this);
void		dtl_av_unshift(dtl_av_t *this, dtl_dv_t *pValue);


//Utility functions
void	dtl_av_extend(dtl_av_t *this, int32_t s32Len);
void	dtl_av_fill(dtl_av_t *this, int32_t s32Len);
void	dtl_av_clear(dtl_av_t *this);
int32_t dtl_av_len(dtl_av_t *this);
bool	dtl_av_exists(dtl_av_t *this, int32_t s32Index);

/*
void dt_av_pushn(context_t *pContext, array_t *pArray, scalar_t **pScalars, uint32_t u32n);			//Pushes u32n SVs to the end of the array
int32_t dtl_av_popn(context_t *pContext, array_t *pArray, scalar_t **pScalars, uint32_t u32n);		//Pops (up to) u32n SVs from the end of the array
int32_t dtl_av_shiftn(context_t *pContext, array_t *pArray, scalar_t **pScalars, uint32_t u32n);		//Shifts (up to) u32n SVs from the start of the array
void dtl_av_unshiftn(context_t *pContext, array_t *pArray, scalar_t **pScalars, uint32_t u32n);		//Unshifts u32n SVs to the start of the array
*/


#endif //DTL_AV_H__
