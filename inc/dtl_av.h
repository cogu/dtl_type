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
#include "adt_ary.h"

typedef struct dtl_av_tag{
  DTL_DV_HEAD(adt_ary_t)
} dtl_av_t;

/***************** Public Function Declarations *******************/
//Constructor/Destructor
dtl_av_t* dtl_av_new();
dtl_av_t* dtl_av_make(dtl_dv_t** ppValue, int32_t s32Len);
void dtl_av_delete(dtl_av_t *self);
void dtl_av_create(dtl_av_t *self);
void dtl_av_destroy(dtl_av_t *self);

//Accessors
dtl_dv_t** dtl_av_set(dtl_av_t *self, int32_t s32Index, dtl_dv_t *pValue);
dtl_dv_t** dtl_av_get(const dtl_av_t *self, int32_t s32Index);
void dtl_av_push(dtl_av_t *self, dtl_dv_t *dv, bool autoIncrementRef);
dtl_dv_t* dtl_av_pop(dtl_av_t *self);
dtl_dv_t* dtl_av_shift(dtl_av_t *self);
void dtl_av_unshift(dtl_av_t *self, dtl_dv_t *pValue);
dtl_dv_t* dtl_av_value(const dtl_av_t *self, int32_t s32Index);


//Utility functions
void dtl_av_extend(dtl_av_t *self, int32_t s32Len);
void dtl_av_fill(dtl_av_t *self, int32_t s32Len);
void dtl_av_clear(dtl_av_t *self);
int32_t dtl_av_length(const dtl_av_t *self);
bool dtl_av_exists(const dtl_av_t *self, int32_t s32Index);


#endif //DTL_AV_H__
