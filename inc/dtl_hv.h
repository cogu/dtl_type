/*****************************************************************************
* \file      dtl_hv.h
* \author    Conny Gustafsson
* \date      2013-07-22
* \brief     DTL Hash
*
* Copyright (c) 2013-2019 Conny Gustafsson
* Permission is hereby granted, free of charge, to any person obtaining a copy of
* this software and associated documentation files (the "Software"), to deal in
* the Software without restriction, including without limitation the rights to
* use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
* the Software, and to permit persons to whom the Software is furnished to do so,
* subject to the following conditions:

* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.

* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
* COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
* IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
* CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
******************************************************************************/
#ifndef DTL_HV_H_
#define DTL_HV_H_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include "dtl_sv.h"
#include "adt_hash.h"
#include "dtl_av.h"

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
typedef struct dtl_hv_tag
{
  DTL_DV_HEAD(adt_hash_t)
} dtl_hv_t;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
//Constructor/Destructor
dtl_hv_t* dtl_hv_new(void);
void dtl_hv_delete(dtl_hv_t *self);
void dtl_hv_create(dtl_hv_t *self);
void dtl_hv_destroy(dtl_hv_t *self);


//Accessors
void dtl_hv_set(dtl_hv_t *self, const char *pKey, dtl_dv_t *dv, bool autoIncrementRef);
dtl_dv_t* dtl_hv_get(const dtl_hv_t *self, const char *pKey);
dtl_dv_t* dtl_hv_remove(dtl_hv_t *self, const char *pKey);
void dtl_hv_iter_init(dtl_hv_t *self);

/**
 * \param[in] this hash
 * \param[out] pKey pointer to key string
 * \param[out] pKeyLen pointer to integer containing length of key string
 * \return pointer to scalar value
 */
dtl_dv_t* dtl_hv_iter_next(dtl_hv_t *self,const char **ppKey);



//Utility functions
uint32_t dtl_hv_length(const dtl_hv_t *self);
bool dtl_hv_exists(const dtl_hv_t *self, const char *pKey);
dtl_av_t* dtl_hv_keys(const dtl_hv_t *self);

#endif //DTL_HV_H_

