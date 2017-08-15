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
void		dtl_hv_delete(dtl_hv_t *this);
void 	dtl_hv_create(dtl_hv_t *this);
void 	dtl_hv_destroy(dtl_hv_t *this);


//Accessors
void	dtl_hv_set(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen, dtl_dv_t *pValue);
dtl_dv_t*	dtl_hv_get(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen);
dtl_dv_t*	dtl_hv_remove(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen);
void	dtl_hv_iter_init(dtl_hv_t *this);

/**
 * \param[in] this hash
 * \param[out] pKey pointer to key string
 * \param[out] pKeyLen pointer to integer containing length of key string
 * \return pointer to scalar value
 */
dtl_dv_t*	dtl_hv_iter_next(dtl_hv_t *this,const char **ppKey,uint32_t *pKeyLen);



//Utility functions
uint32_t 	dtl_hv_size(dtl_hv_t *this);
bool		dtl_hv_exists(dtl_hv_t *this, const char *pKey, uint32_t u32KeyLen);
uint32_t	dtl_hv_keys(dtl_hv_t *this,dtl_av_t *pArray);



#endif //DTL_HV__
