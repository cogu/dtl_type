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
#include "CMemLeak.h"

/**************** Public Variable Declarations *******************/
static dtl_svx_t g_dtl_svx_undef = {0};
dtl_sv_t g_dtl_sv_undef = {&g_dtl_svx_undef,1,((uint32_t)DTL_DV_SCALAR)};

/**************** Private Function Declarations *******************/
static void dtl_sv_set_type(dtl_sv_t *this,dtl_sv_type_id type);
static void dtl_sv_ztrim(char *str);

/**************** Private Variable Declarations *******************/


/****************** Public Function Definitions *******************/
//Constructor/Destructor
dtl_sv_t* dtl_sv_new(void){
	dtl_sv_t *this;
	if((this = (dtl_sv_t*)malloc(sizeof(dtl_sv_t)))==(dtl_sv_t*)0){
		return (dtl_sv_t*)0;
	}
	dtl_sv_create(this);
	return this;
}

void dtl_sv_delete(dtl_sv_t* this){
	if(this){
		dtl_sv_destroy(this);
		free(this);
	}
}

void dtl_sv_create(dtl_sv_t* this){
	if(this){
		this->pAny = (dtl_svx_t*) malloc(sizeof(dtl_svx_t));
		assert(this->pAny);
		this->u32Flags = ((uint32_t)DTL_DV_SCALAR);
		this->u32RefCnt = 1;
	}
}

void dtl_sv_destroy(dtl_sv_t* this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_CSTR:
			if(this->pAny->cstr) free(this->pAny->cstr);
			break;
		case DTL_SV_PTR:
			break;
			if(this->pAny->ptr.pDestructor){
				this->pAny->ptr.pDestructor(this->pAny->ptr.p);
			}
			break;
		case DTL_SV_DV:
			if(this->pAny->dv){
				dtl_dv_ref_dec(this->pAny->dv);
			}
			break;
		default:
			break;
		}
		free(this->pAny);
		this->pAny = 0;
	}
}

dtl_sv_t *dtl_sv_make_i32(int32_t i32){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_i32(this,i32);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_u32(uint32_t u32){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_u32(this,u32);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_i64(int64_t i64){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_i64(this,i64);
	}
	return this;
}
dtl_sv_t *dtl_sv_make_u64(uint64_t u64){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_u64(this,u64);
	}
	return this;
}


dtl_sv_t *dtl_sv_make_flt(float flt){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_flt(this,flt);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_dbl(double dbl){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_dbl(this,dbl);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_bool(bool bl){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_bool(this,bl);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_ptr(void *ptr,void (*pDestructor)(void*)){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_ptr(this,ptr,pDestructor);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_cstr(const char* str){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_cstr(this,str);
	}
	return this;
}

dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv){
	dtl_sv_t *this = dtl_sv_new();
	if(this){
		dtl_sv_set_dv(this,dv);
	}
	return this;
}


dtl_sv_type_id dtl_sv_type(dtl_sv_t* this){
	if(this){
		uint8_t u8Type = (uint8_t) ((this->u32Flags & DTL_SV_TYPE_MASK)>>DTL_SV_TYPE_SHIFT);
		return (dtl_sv_type_id) u8Type;
	}
	return DTL_SV_UNDEF;
}

//Setters
void dtl_sv_set_i32(dtl_sv_t *this, int32_t i32){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_I32);
		this->pAny->i32 = i32;
	}
}

void dtl_sv_set_u32(dtl_sv_t *this, uint32_t u32){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_U32);
		this->pAny->u32 = u32;
	}
}

void dtl_sv_set_i64(dtl_sv_t *this, int64_t i64){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_I64);
		this->pAny->i64 = i64;
	}
}

void dtl_sv_set_u64(dtl_sv_t *this, uint64_t u64){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_U64);
		this->pAny->u64 = u64;
	}
}


void dtl_sv_set_flt(dtl_sv_t *this, float flt){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_FLT);
		this->pAny->flt = flt;
	}
}
void dtl_sv_set_dbl(dtl_sv_t *this, double dbl){
	if(this){
		dtl_sv_set_type(this,DTL_SV_DBL);
		this->pAny->dbl = dbl;
	}
}

void dtl_sv_set_bool(dtl_sv_t *this, bool bl){
	if(this){
		dtl_sv_set_type(this,DTL_SV_BOOL);
		this->pAny->bl = bl;
	}
}

void dtl_sv_set_ptr(dtl_sv_t *this, void *p, void (*pDestructor)(void*)){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_PTR);
		this->pAny->ptr.p = p;
		this->pAny->ptr.pDestructor = pDestructor;
	}
}

void dtl_sv_set_cstr(dtl_sv_t *this, const char* str){
	if(this){
		dtl_sv_set_type(this,DTL_SV_CSTR);
		this->pAny->cstr = strdup(str);
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

void dtl_sv_set_dv(dtl_sv_t *this, dtl_dv_t *dv){
	if(this){
		if(dtl_sv_type(this)==DTL_SV_DV){
			dtl_dv_ref_dec(this->pAny->dv);
		}
		dtl_sv_set_type(this,DTL_SV_DV);
		this->pAny->dv = dv;
		dtl_dv_ref_inc(dv);
	}
}


//Getters
int32_t dtl_sv_get_i32(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return this->pAny->i32;
		case DTL_SV_U32:
			return (int32_t) this->pAny->u32;
		case DTL_SV_I64:
			return (int32_t) this->pAny->i64;
		case DTL_SV_U64:
			return (int32_t) this->pAny->u64;
		case DTL_SV_FLT:
			return (int32_t) this->pAny->flt;
		case DTL_SV_DBL:
			return (int32_t) this->pAny->dbl;
		case DTL_SV_BOOL:
			return (int32_t) this->pAny->bl;
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
uint32_t dtl_sv_get_u32(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (uint32_t) this->pAny->i32;
		case DTL_SV_U32:
			return this->pAny->u32;
		case DTL_SV_I64:
			return (uint32_t) this->pAny->i64;
		case DTL_SV_U64:
			return (uint32_t) this->pAny->u64;
		case DTL_SV_FLT:
			return (uint32_t) this->pAny->flt;
		case DTL_SV_DBL:
			return (uint32_t) this->pAny->dbl;
		case DTL_SV_BOOL:
			return (uint32_t) this->pAny->bl;
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

int64_t dtl_sv_get_i64(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (int64_t) this->pAny->i32;
		case DTL_SV_U32:
			return (int64_t) this->pAny->u32;
		case DTL_SV_I64:
			return this->pAny->i64;
		case DTL_SV_U64:
			return (int64_t) this->pAny->u64;
		case DTL_SV_FLT:
			return (int64_t) this->pAny->flt;
		case DTL_SV_DBL:
			return (int64_t) this->pAny->dbl;
		case DTL_SV_BOOL:
			return (int64_t) this->pAny->bl;
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

uint64_t dtl_sv_get_u64(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (uint64_t) this->pAny->i32;
		case DTL_SV_U32:
			return (uint64_t) this->pAny->u32;
		case DTL_SV_I64:
			return (uint64_t) this->pAny->i64;
		case DTL_SV_U64:
			return this->pAny->u64;
		case DTL_SV_FLT:
			return (uint64_t) this->pAny->flt;
		case DTL_SV_DBL:
			return (uint64_t) this->pAny->dbl;
		case DTL_SV_BOOL:
			return (uint64_t) this->pAny->bl;
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


float dtl_sv_get_flt(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (float) this->pAny->i32;
		case DTL_SV_U32:
			return (float) this->pAny->u32;
		case DTL_SV_I64:
			return (float) this->pAny->i64;
		case DTL_SV_U64:
			return (float) this->pAny->u64;
		case DTL_SV_FLT:
			return this->pAny->flt;
		case DTL_SV_DBL:
			return (float) this->pAny->dbl;
		case DTL_SV_BOOL:
			return (float) this->pAny->bl;
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

double dtl_sv_get_dbl(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (double) this->pAny->i32;
		case DTL_SV_U32:
			return (double) this->pAny->u32;
		case DTL_SV_FLT:
			return (double) this->pAny->flt;
		case DTL_SV_I64:
			return (double) this->pAny->i64;
		case DTL_SV_U64:
			return (double) this->pAny->u64;
		case DTL_SV_DBL:
			return this->pAny->dbl;
		case DTL_SV_BOOL:
			return (double) this->pAny->bl;
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

bool dtl_sv_get_bool(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (bool) this->pAny->i32;
		case DTL_SV_U32:
			return (bool) this->pAny->u32;
		case DTL_SV_I64:
			return (bool) this->pAny->i64;
		case DTL_SV_U64:
			return (bool) this->pAny->u64;
		case DTL_SV_FLT:
			return (bool) this->pAny->flt;
		case DTL_SV_DBL:
			return (bool) this->pAny->dbl;
		case DTL_SV_BOOL:
			return this->pAny->bl;
		case DTL_SV_CSTR:
			if(strcmp(this->pAny->cstr,"true")==0){
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

void* dtl_sv_get_ptr(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
		case DTL_SV_UNDEF:
			break;
		case DTL_SV_I32:
			return (void*) this->pAny->i32;
		case DTL_SV_U32:
			return (void*) this->pAny->u32;
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
			return (void*) this->pAny->cstr;
			break;
		case DTL_SV_PTR:
			return this->pAny->ptr.p;
			break;
		case DTL_SV_DV:
			return (void*) this->pAny->dv;
			break;
		}
	}
	return (void*)0;
}

const char* dtl_sv_get_cstr(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
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
			return this->pAny->bl? "true" : "false";
		case DTL_SV_CSTR:
			return this->pAny->cstr;
			break;
		case DTL_SV_PTR:
			break;
		case DTL_SV_DV:
			break;
		}
	}
	return (const char*)0;
}

dtl_dv_t *dtl_sv_get_dv(dtl_sv_t *this){
	if(this){
		switch(dtl_sv_type(this)){
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
			return this->pAny->dv;
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
				sprintf(numBuf,"%lld",dtl_sv_get_i64(sv));
				break;
			case DTL_SV_U64:
				isNum = 1;
				sprintf(numBuf,"%llu",dtl_sv_get_u64(sv));
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
void dtl_sv_set_type(dtl_sv_t *this,dtl_sv_type_id type){
	this->u32Flags &= ~((uint32_t)DTL_SV_TYPE_MASK);
	this->u32Flags |= (((uint32_t)type)<<DTL_SV_TYPE_SHIFT) & DTL_SV_TYPE_MASK;
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
