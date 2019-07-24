/*****************************************************************************
* \file:   		dtl_hv.h
* \author:		Conny Gustafsson
* \date:		2013-07-22
* \brief:		DTL hash
*
* Copyright (c) 2013 Conny Gustafsson
*
******************************************************************************/
#ifndef DTL_HV__
#define DTL_HV__
#include "dtl_sv.h"
#include "adt_hash.h"
#include "dtl_av.h"

typedef struct dtl_hv_tag{
  DTL_DV_HEAD(adt_hash_t)
} dtl_hv_t;

/***************** Public Function Declarations *******************/
//Constructor/Destructor
dtl_hv_t*	dtl_hv_new(void);
void		dtl_hv_delete(dtl_hv_t *self);
void 	dtl_hv_create(dtl_hv_t *self);
void 	dtl_hv_destroy(dtl_hv_t *self);


//Accessors
void	dtl_hv_set(dtl_hv_t *self, const char *pKey, dtl_dv_t *pValue);
dtl_dv_t*	dtl_hv_get(const dtl_hv_t *self, const char *pKey);
dtl_dv_t*	dtl_hv_remove(dtl_hv_t *self, const char *pKey);
void	dtl_hv_iter_init(dtl_hv_t *self);

/**
 * \param[in] this hash
 * \param[out] pKey pointer to key string
 * \param[out] pKeyLen pointer to integer containing length of key string
 * \return pointer to scalar value
 */
dtl_dv_t*	dtl_hv_iter_next(dtl_hv_t *self,const char **ppKey);



//Utility functions
uint32_t 	dtl_hv_length(const dtl_hv_t *self);
bool		dtl_hv_exists(const dtl_hv_t *self, const char *pKey);
int32_t	dtl_hv_keys(const dtl_hv_t *self, adt_ary_t *pArray);


#endif //DTL_HV__
