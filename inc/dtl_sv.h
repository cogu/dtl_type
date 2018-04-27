/*****************************************************************************
* \file:   		dtl_sv.h
* \author:		Conny Gustafsson
* \date:		2013-03-07
* \brief:		DTL Scalar
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#ifndef DTL_SV_H__
#define DTL_SV_H__
#include "dtl_dv.h"
#include "adt_str.h"
#include <stdbool.h>

#define DTL_SV_TYPE_MASK 		0xF0
#define DTL_SV_TYPE_SHIFT 		4

typedef struct dtl_pv_tag{
	void *p;
	void (*pDestructor)(void*);
}dtl_pv_t;

typedef union sv_tag{
    int32_t		i32;
    uint32_t	u32;
    int64_t		i64;
    uint64_t	u64;
    float  		flt;
    double  	dbl;
    char * 		cstr;
    dtl_pv_t	ptr;
    dtl_dv_t*	dv;
    bool		bl;
} dtl_svx_t;


typedef struct dtl_sv_tag{
	DTL_DV_HEAD(dtl_svx_t)
}dtl_sv_t;

typedef enum dtl_sv_type_tag{
	DTL_SV_UNDEF = 0,
	DTL_SV_I32,
	DTL_SV_U32,
	DTL_SV_I64,
	DTL_SV_U64,
	DTL_SV_FLT,
	DTL_SV_DBL,
	DTL_SV_BOOL,
	DTL_SV_CSTR,
	DTL_SV_PTR,
	DTL_SV_DV,
} dtl_sv_type_id;


/**************** Public Variable Declarations *******************/
extern dtl_sv_t g_dtl_sv_undef;


/***************** Public Function Declarations *******************/
//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void);
void dtl_sv_delete(dtl_sv_t* self);
void dtl_sv_create(dtl_sv_t* self);
void dtl_sv_destroy(dtl_sv_t* self);
dtl_sv_t *dtl_sv_make_i32(int32_t i32);
dtl_sv_t *dtl_sv_make_u32(uint32_t u32);
dtl_sv_t *dtl_sv_make_i64(int64_t i64);
dtl_sv_t *dtl_sv_make_u64(uint64_t u64);
dtl_sv_t *dtl_sv_make_flt(float sf);
dtl_sv_t *dtl_sv_make_dbl(double df);
dtl_sv_t *dtl_sv_make_bool(bool bl);
dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*));
dtl_sv_t *dtl_sv_make_cstr(const char* str);
dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv);

dtl_sv_type_id dtl_sv_type(const dtl_sv_t* self);

//Setters
void dtl_sv_set_i32(dtl_sv_t *self, int32_t i32);
void dtl_sv_set_u32(dtl_sv_t *self, uint32_t u32);
void dtl_sv_set_i64(dtl_sv_t *self, int64_t i64);
void dtl_sv_set_u64(dtl_sv_t *self, uint64_t u64);
void dtl_sv_set_flt(dtl_sv_t *self, float flt);
void dtl_sv_set_dbl(dtl_sv_t *self, double dbl);
void dtl_sv_set_bool(dtl_sv_t *self, bool bl);
void dtl_sv_set_ptr(dtl_sv_t *self, void *p, void (*pDestructor)(void*));
void dtl_sv_set_cstr(dtl_sv_t *self, const char* str);
void dtl_sv_set_bstr(dtl_sv_t *self, const char *pBegin, const char *pEnd);
void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv);



//Getters
int32_t dtl_sv_get_i32(const dtl_sv_t *self);
uint32_t dtl_sv_get_u32(const dtl_sv_t *self);
int64_t dtl_sv_get_i64(const dtl_sv_t *self);
uint64_t dtl_sv_get_u64(const dtl_sv_t *self);
float dtl_sv_get_flt(const dtl_sv_t *self);
double dtl_sv_get_dbl(const dtl_sv_t *self);
bool dtl_sv_get_bool(const dtl_sv_t *self);
void* dtl_sv_get_ptr(const dtl_sv_t *self);
const char *dtl_sv_get_cstr(const dtl_sv_t *self);
dtl_dv_t *dtl_sv_get_dv(const dtl_sv_t *self);
void dtl_sv_to_string(const dtl_sv_t *self,adt_str_t *str);

//Macros
#define dtl_sv_undef() &g_dtl_sv_undef

#endif //DTL_SV_H__
