/*****************************************************************************
* \file:   		dtl_sv.c
* \author:		Conny Gustafsson
* \date:		2013-07-22
* \brief:		DTL scalar
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include "dtl_sv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

#ifdef _MSC_VER
#define STRDUP _strdup
#else
#define STRDUP strdup
#endif


/**************** Public Variable Declarations *******************/
static dtl_svx_t g_dtl_svx_undef = {0};
dtl_sv_t g_dtl_sv_undef = {&g_dtl_svx_undef,1,((uint32_t)DTL_DV_SCALAR)};

/**************** Private Function Declarations *******************/
static void dtl_sv_set_type(dtl_sv_t *self,dtl_sv_type_id type);
static void dtl_sv_ztrim(char *str);

/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void){
	dtl_sv_t *self = (dtl_sv_t*)malloc(sizeof(dtl_sv_t));
	if(self !=(dtl_sv_t*)0){
		dtl_sv_create(self);
	}
	return self;
}

void dtl_sv_delete(dtl_sv_t* self){
	if(self){
		dtl_sv_destroy(self);
		free(self);
	}
}

void dtl_sv_create(dtl_sv_t* self){
	if(self){
		self->pAny = (dtl_svx_t*) malloc(sizeof(dtl_svx_t));
		if (self->pAny != 0)
		{
		   memset(self->pAny, 0, sizeof(dtl_svx_t));
	      self->u32Flags = ((uint32_t)DTL_DV_SCALAR);
	      self->u32RefCnt = 1;
		}
		else
		{
		   assert(0);
		}
	}
}

void dtl_sv_destroy(dtl_sv_t* self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_CSTR:
			if(self->pAny->cstr) free(self->pAny->cstr);
			break;
		case DTL_SV_PTR:
			if(self->pAny->ptr.pDestructor){
				self->pAny->ptr.pDestructor(self->pAny->ptr.p);
			}
			break;
		case DTL_SV_DV:
			if(self->pAny->dv){
				dtl_dv_ref_dec(self->pAny->dv);
			}
			break;
		default:
			break;
		}
		free(self->pAny);
		self->pAny = 0;
	}
}

dtl_sv_t *dtl_sv_make_i32(int32_t i32){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_i32(self,i32);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_u32(uint32_t u32){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_u32(self,u32);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_i64(int64_t i64){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_i64(self,i64);
	}
	return self;
}
dtl_sv_t *dtl_sv_make_u64(uint64_t u64){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_u64(self,u64);
	}
	return self;
}


dtl_sv_t *dtl_sv_make_flt(float flt){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_flt(self,flt);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_dbl(double dbl){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_dbl(self,dbl);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_bool(bool bl){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_bool(self,bl);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*)){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_ptr(self,ptr,pDestructor);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_cstr(const char* str){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_cstr(self,str);
	}
	return self;
}

dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv){
	dtl_sv_t *self = dtl_sv_new();
	if(self){
		dtl_sv_set_dv(self,dv);
	}
	return self;
}


dtl_sv_type_id dtl_sv_type(dtl_sv_t* self){
	if(self){
		uint8_t u8Type = (uint8_t) ((self->u32Flags & DTL_SV_TYPE_MASK)>>DTL_SV_TYPE_SHIFT);
		return (dtl_sv_type_id) u8Type;
	}
	return DTL_SV_UNDEF;
}

//Setters
void dtl_sv_set_i32(dtl_sv_t *self, int32_t i32){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_I32);
		self->pAny->i32 = i32;
	}
}

void dtl_sv_set_u32(dtl_sv_t *self, uint32_t u32){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_U32);
		self->pAny->u32 = u32;
	}
}

void dtl_sv_set_i64(dtl_sv_t *self, int64_t i64){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_I64);
		self->pAny->i64 = i64;
	}
}

void dtl_sv_set_u64(dtl_sv_t *self, uint64_t u64){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_U64);
		self->pAny->u64 = u64;
	}
}


void dtl_sv_set_flt(dtl_sv_t *self, float flt){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_FLT);
		self->pAny->flt = flt;
	}
}
void dtl_sv_set_dbl(dtl_sv_t *self, double dbl){
	if(self){
		dtl_sv_set_type(self,DTL_SV_DBL);
		self->pAny->dbl = dbl;
	}
}

void dtl_sv_set_bool(dtl_sv_t *self, bool bl){
	if(self){
		dtl_sv_set_type(self,DTL_SV_BOOL);
		self->pAny->bl = bl;
	}
}

void dtl_sv_set_ptr(dtl_sv_t *self, void *p, void (*pDestructor)(void*)){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_PTR);
		self->pAny->ptr.p = p;
		self->pAny->ptr.pDestructor = pDestructor;
	}
}

void dtl_sv_set_cstr(dtl_sv_t *self, const char* str){
	if(self){
		dtl_sv_set_type(self,DTL_SV_CSTR);
		if (self->pAny->cstr != 0){
		   free(self->pAny->cstr);
		}
		self->pAny->cstr = STRDUP(str);
	}
}

void dtl_sv_set_bstr(dtl_sv_t *self, const char *pBegin, const char *pEnd){
   if ( (self != 0) && (pBegin != 0) && (pEnd != 0) && (pBegin<=pEnd) ){
      uint32_t len = (uint32_t) (pEnd-pBegin);
      char *str = (char*) malloc(len+1);
      if(self != 0){
         dtl_sv_set_type(self,DTL_SV_CSTR);
         memcpy(str,pBegin,len);
         str[len]=0;
         self->pAny->cstr = str;
      }
   }
}

void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv){
	if(self){
		if(dtl_sv_type(self)==DTL_SV_DV){
			dtl_dv_ref_dec(self->pAny->dv);
		}
		dtl_sv_set_type(self,DTL_SV_DV);
		self->pAny->dv = dv;
		dtl_dv_ref_inc(dv);
	}
}


//Getters
int32_t dtl_sv_get_i32(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return self->pAny->i32;
		case DTL_SV_U32:
			return (int32_t) self->pAny->u32;
		case DTL_SV_I64:
			return (int32_t) self->pAny->i64;
		case DTL_SV_U64:
			return (int32_t) self->pAny->u64;
		case DTL_SV_FLT:
			return (int32_t) self->pAny->flt;
		case DTL_SV_DBL:
			return (int32_t) self->pAny->dbl;
		case DTL_SV_BOOL:
			return (int32_t) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0;
}
uint32_t dtl_sv_get_u32(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (uint32_t) self->pAny->i32;
		case DTL_SV_U32:
			return self->pAny->u32;
		case DTL_SV_I64:
			return (uint32_t) self->pAny->i64;
		case DTL_SV_U64:
			return (uint32_t) self->pAny->u64;
		case DTL_SV_FLT:
			return (uint32_t) self->pAny->flt;
		case DTL_SV_DBL:
			return (uint32_t) self->pAny->dbl;
		case DTL_SV_BOOL:
			return (uint32_t) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0;
}

int64_t dtl_sv_get_i64(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (int64_t) self->pAny->i32;
		case DTL_SV_U32:
			return (int64_t) self->pAny->u32;
		case DTL_SV_I64:
			return self->pAny->i64;
		case DTL_SV_U64:
			return (int64_t) self->pAny->u64;
		case DTL_SV_FLT:
			return (int64_t) self->pAny->flt;
		case DTL_SV_DBL:
			return (int64_t) self->pAny->dbl;
		case DTL_SV_BOOL:
			return (int64_t) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0;
}

uint64_t dtl_sv_get_u64(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (uint64_t) self->pAny->i32;
		case DTL_SV_U32:
			return (uint64_t) self->pAny->u32;
		case DTL_SV_I64:
			return (uint64_t) self->pAny->i64;
		case DTL_SV_U64:
			return self->pAny->u64;
		case DTL_SV_FLT:
			return (uint64_t) self->pAny->flt;
		case DTL_SV_DBL:
			return (uint64_t) self->pAny->dbl;
		case DTL_SV_BOOL:
			return (uint64_t) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0;
}


float dtl_sv_get_flt(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (float) self->pAny->i32;
		case DTL_SV_U32:
			return (float) self->pAny->u32;
		case DTL_SV_I64:
			return (float) self->pAny->i64;
		case DTL_SV_U64:
			return (float) self->pAny->u64;
		case DTL_SV_FLT:
			return self->pAny->flt;
		case DTL_SV_DBL:
			return (float) self->pAny->dbl;
		case DTL_SV_BOOL:
			return (float) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0.0;
}

double dtl_sv_get_dbl(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (double) self->pAny->i32;
		case DTL_SV_U32:
			return (double) self->pAny->u32;
		case DTL_SV_FLT:
			return (double) self->pAny->flt;
		case DTL_SV_I64:
			return (double) self->pAny->i64;
		case DTL_SV_U64:
			return (double) self->pAny->u64;
		case DTL_SV_DBL:
			return self->pAny->dbl;
		case DTL_SV_BOOL:
			return (double) self->pAny->bl;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return 0.0;
}

bool dtl_sv_get_bool(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (bool) self->pAny->i32;
		case DTL_SV_U32:
			return (bool) self->pAny->u32;
		case DTL_SV_I64:
			return (bool) self->pAny->i64;
		case DTL_SV_U64:
			return (bool) self->pAny->u64;
		case DTL_SV_FLT:
			return (bool) self->pAny->flt;
		case DTL_SV_DBL:
			return (bool) self->pAny->dbl;
		case DTL_SV_BOOL:
			return self->pAny->bl;
		case DTL_SV_CSTR:
			if(strcmp(self->pAny->cstr,"true")==0){
				return true;
			}
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return false;
}

void* dtl_sv_get_ptr(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (void*) ((long) self->pAny->i32);
		case DTL_SV_U32:
			return (void*) ((long) self->pAny->u32);
		case DTL_SV_I64:
			break;
		case DTL_SV_U64:
			break;
		case DTL_SV_FLT:
			break;
		case DTL_SV_DBL:
			break;
		case DTL_SV_BOOL:
			break;
		case DTL_SV_CSTR:
			return (void*) self->pAny->cstr;
			break;
		case DTL_SV_PTR:
			return self->pAny->ptr.p;
			break;
		case DTL_SV_DV:
			return (void*) self->pAny->dv;
			break;
		}
	}
	return (void*)0;
}

const char* dtl_sv_get_cstr(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			break;
		case DTL_SV_U32:
			break;
		case DTL_SV_I64:
			break;
		case DTL_SV_U64:
			break;
		case DTL_SV_FLT:
			break;
		case DTL_SV_DBL:
			break;
		case DTL_SV_BOOL:
			return self->pAny->bl? "true" : "false";
		case DTL_SV_CSTR:
			return self->pAny->cstr;
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return (const char*)0;
}

dtl_dv_t *dtl_sv_get_dv(dtl_sv_t *self){
	if(self){
		switch(dtl_sv_type(self)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			break;
		case DTL_SV_U32:
			break;
		case DTL_SV_I64:
			break;
		case DTL_SV_U64:
			break;
		case DTL_SV_FLT:
			break;
		case DTL_SV_DBL:
			break;
		case DTL_SV_BOOL:
			break;
		case DTL_SV_CSTR:
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			return self->pAny->dv;
			break;
		}
	}
	return (dtl_dv_t*)0;
}

void dtl_sv_to_string(dtl_sv_t *sv,adt_str_t *str){
	if(sv && str){
		adt_str_clear(str);
		char *numBuf = (char*) malloc(1080);
		if(numBuf){
			int isNum = 0;
			switch(dtl_sv_type(sv)){
			case DTL_SV_UNDEF:
				adt_str_append_cstr(str,"(undefined)");
				break;
			case DTL_SV_I32:
				isNum = 1;
				sprintf(numBuf,"%d",dtl_sv_get_i32(sv));
				break;
			case DTL_SV_U32:
				isNum = 1;
				sprintf(numBuf,"%u",dtl_sv_get_u32(sv));
				break;
			case DTL_SV_I64:
				isNum = 1;
				sprintf(numBuf,"%lld", (long long int) dtl_sv_get_i64(sv));
				break;
			case DTL_SV_U64:
				isNum = 1;
				sprintf(numBuf,"%llu", (long long unsigned int) dtl_sv_get_u64(sv));
				break;
			case DTL_SV_FLT:
				isNum = 1;
				sprintf(numBuf,"%f",dtl_sv_get_flt(sv));
				dtl_sv_ztrim(numBuf);
				break;
			case DTL_SV_DBL:
				isNum = 1;
				sprintf(numBuf,"%f",dtl_sv_get_dbl(sv));
				dtl_sv_ztrim(numBuf);
				break;
			case DTL_SV_BOOL:
				if(dtl_sv_get_bool(sv)){
					adt_str_append_cstr(str,"true");
				}
				else{
					adt_str_append_cstr(str,"false");
				}
				break;
			case DTL_SV_CSTR:
				adt_str_push(str,'"');
				adt_str_append_cstr(str,dtl_sv_get_cstr(sv));
				adt_str_push(str,'"');
				break;
			case DTL_SV_PTR:
				isNum = 1;
				sprintf(numBuf,"%p",dtl_sv_get_ptr(sv));
				break;
			case DTL_SV_DV:
				break;
			}
			if(isNum){
				adt_str_append_cstr(str,numBuf);
			}
			free(numBuf);
		}
	}
}


/***************** Private Function Definitions *******************/
void dtl_sv_set_type(dtl_sv_t *self,dtl_sv_type_id type){
	self->u32Flags &= ~((uint32_t)DTL_SV_TYPE_MASK);
	self->u32Flags |= (((uint32_t)type)<<DTL_SV_TYPE_SHIFT) & DTL_SV_TYPE_MASK;
}

void dtl_sv_ztrim(char *str){
	char *begin = str;
	char *end = begin+strlen(str);
	char *p = begin;
	char *a=0,*b=0;
	while( p<end){
		if(!isdigit(*p)){
			if(*p == '.'){
				a = p;
				break;
			}
			else{
				return; //not a number
			}
		}
		++p;
	}
	p=end-1;
	while(p>a){
		if(*p == '0'){
			b=p;
		}
		else{
			break;
		}
		--p;
	}
	if(a && b){
		assert(b>a);
		if(b-1==a){
			--b; //nothing on the right side of the '.'
		}
		*b=0;
	}
	if(strlen(str)==0){
		//empty string, replace with 0
		strcpy(str,"0");
	}
}
