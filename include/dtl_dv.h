/*****************************************************************************
* \file      dtl_dv.h
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     Dynamic Value base type
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/
#ifndef DTL_DV_H_
#define DTL_DV_H_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define DTL_DV_TYPE_MASK      0xFu
#define DTL_DV_TYPE_SHIFT     0u

#define DTL_DV_HEAD(ValueType)\
   ValueType *pAny;\
   uint32_t u32RefCnt;\
   uint32_t u32Flags;

/**
 * \brief Base dynamic value type.
 */
typedef struct dtl_dv_tag {
   DTL_DV_HEAD(void)
} dtl_dv_t;

/**
 * \brief Type identifier for dynamic values.
 */
typedef enum dtl_dv_type_id {
   DTL_DV_INVALID = 0,
   DTL_DV_NULL,
   DTL_DV_SCALAR,
   DTL_DV_ARRAY,
   DTL_DV_HASH,
} dtl_dv_type_id;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

/**
 * \brief Creates a new dynamic value representing NULL.
 *
 * \return Pointer to newly created dtl_dv_t, or NULL on allocation failure.
 */
dtl_dv_t *dtl_dv_null(void);

/**
 * \brief Deletes a dynamic value based on its concrete type.
 *
 * \param dv Pointer to dynamic value to delete.
 */
void dtl_dv_delete(dtl_dv_t *dv);

/**
 * \brief Void wrapper for dtl_dv_delete (useful for destructor callbacks).
 *
 * \param arg Pointer to dynamic value to delete.
 */
void dtl_dv_vdelete(void *arg);

/**
 * \brief Increments reference count of dynamic value.
 *
 * \param dv Pointer to dynamic value.
 */
void dtl_dv_inc_ref(dtl_dv_t *dv);

/**
 * \brief Decrements reference count of dynamic value and frees it when reaching zero.
 *
 * \param dv Pointer to dynamic value.
 */
void dtl_dv_dec_ref(dtl_dv_t *dv);

/**
 * \brief Gets the concrete type ID of a dynamic value.
 *
 * \param dv Pointer to dynamic value.
 * \return Type ID, or DTL_DV_INVALID if dv is NULL.
 */
dtl_dv_type_id dtl_dv_type(const dtl_dv_t *dv);

/**
 * \brief Void wrapper for dtl_dv_dec_ref.
 *
 * \param ptr Pointer to dynamic value.
 */
void dtl_dv_dec_ref_void(void *ptr);

#define dtl_ref_cnt(dv) ((dv)->u32RefCnt)
#define dtl_inc_ref(dv) dtl_dv_inc_ref((dtl_dv_t *)(dv))
#define dtl_dec_ref(dv) dtl_dv_dec_ref((dtl_dv_t *)(dv))

#ifdef __cplusplus
}
#endif

#endif // DTL_DV_H_
