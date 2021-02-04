/*****************************************************************************
* \file:   		dtl_dv.h
* \author:		Conny Gustafsson
* \date:		2013-03-07
* \brief:		Dynamic Value
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#ifndef DTL_DV_H__
#define DTL_DV_H__
#include <stdint.h>

#define DTL_DV_TYPE_MASK 		0xF
#define DTL_DV_TYPE_SHIFT 		0

#define DTL_DV_HEAD(ValueType)\
	ValueType *pAny;\
	uint32_t u32RefCnt;\
	uint32_t u32Flags;


typedef struct dtl_dv_tag{
	DTL_DV_HEAD(void)
} dtl_dv_t;

typedef enum dtl_dv_type_id{
	DTL_DV_INVALID=0, //TODO: THIS SHOULD BE REMOVED IN A FUTURE VERSION. USE DTL_DV_NULL INSTEAD.
	DTL_DV_NULL,
	DTL_DV_SCALAR,
	DTL_DV_ARRAY,
	DTL_DV_HASH,
} dtl_dv_type_id;


/***************** Public Function Declarations *******************/
dtl_dv_t *dtl_dv_null();
void dtl_dv_delete(dtl_dv_t* dv );
void dtl_dv_vdelete(void *arg);
void dtl_dv_inc_ref(dtl_dv_t* dv);
void dtl_dv_dec_ref(dtl_dv_t* dv);
dtl_dv_type_id dtl_dv_type(const dtl_dv_t* dv);

void dtl_dv_dec_ref_void(void* ptr);

#define dtl_ref_cnt(dv) (dv->u32RefCnt)
#define dtl_inc_ref(dv) dtl_dv_inc_ref((dtl_dv_t*)dv)
#define dtl_dec_ref(dv) dtl_dv_dec_ref((dtl_dv_t*)dv)

#endif //DTL_DV_H__
