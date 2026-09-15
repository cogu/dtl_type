/*****************************************************************************
* \file      dtl_hv.h
* \author    Conny Gustafsson
* \date      2013-07-22
* \brief     DTL Hash Value
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/
#ifndef DTL_HV_H_
#define DTL_HV_H_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include "dtl_sv.h"
#include "adt_hash.h"
#include "dtl_av.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

/**
 * \brief Dynamic Hash Value type.
 */
typedef struct dtl_hv_tag {
   DTL_DV_HEAD(adt_hash_t)
} dtl_hv_t;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

// Constructor/Destructor

/**
 * \brief Allocates and initializes a new empty hash.
 * \return Pointer to new hash, or NULL on allocation failure.
 */
dtl_hv_t *dtl_hv_new(void);

/**
 * \brief Destroys and frees a hash.
 * \param self Pointer to hash.
 */
void dtl_hv_delete(dtl_hv_t *self);

/**
 * \brief Initializes a pre-allocated hash.
 * \param self Pointer to hash.
 */
void dtl_hv_create(dtl_hv_t *self);

/**
 * \brief Destroys contents of a hash.
 * \param self Pointer to hash.
 */
void dtl_hv_destroy(dtl_hv_t *self);

// Accessors

/**
 * \brief Stores a dynamic value at a C string key.
 *
 * If a previous value was stored at key, its reference count is decremented.
 *
 * \param self Pointer to hash.
 * \param key C string key.
 * \param dv Dynamic value to store.
 * \param auto_increment_ref If true, increments reference count of dv.
 */
void dtl_hv_set_cstr(dtl_hv_t *self, const char *key, dtl_dv_t *dv, bool auto_increment_ref);

/**
 * \brief Retrieves the dynamic value associated with a C string key.
 * \param self Pointer to hash.
 * \param key C string key.
 * \return Stored dynamic value, or NULL if key does not exist.
 */
dtl_dv_t *dtl_hv_get_cstr(const dtl_hv_t *self, const char *key);

/**
 * \brief Removes and returns the dynamic value associated with a C string key.
 * \param self Pointer to hash.
 * \param key C string key.
 * \return Removed dynamic value, or NULL if key does not exist.
 */
dtl_dv_t *dtl_hv_remove_cstr(dtl_hv_t *self, const char *key);

/**
 * \brief Initializes the hash iterator to the beginning.
 * \param self Pointer to hash.
 */
void dtl_hv_iter_init(dtl_hv_t *self);

/**
 * \brief Advances hash iterator and returns the next dynamic value and key.
 * \param self Pointer to hash.
 * \param key Optional output pointer receiving the current item's key string.
 * \return Pointer to next dynamic value, or NULL when iteration ends.
 */
dtl_dv_t *dtl_hv_iter_next_cstr(dtl_hv_t *self, const char **key);

// Utility functions

/**
 * \brief Returns number of key-value pairs in the hash.
 * \param self Pointer to hash.
 * \return Number of items in hash, or 0 if self is NULL.
 */
uint32_t dtl_hv_length(const dtl_hv_t *self);

/**
 * \brief Checks if a key exists in the hash.
 * \param self Pointer to hash.
 * \param key C string key.
 * \return true if key exists, false otherwise.
 */
bool dtl_hv_exists_cstr(const dtl_hv_t *self, const char *key);

/**
 * \brief Returns a new array containing all keys found in the hash as string scalars.
 *
 * Caller is responsible for disposing the returned array (e.g. via dtl_dec_ref).
 *
 * \param self Pointer to hash.
 * \return Pointer to newly created dtl_av_t array of keys, or NULL on failure.
 */
dtl_av_t *dtl_hv_keys(const dtl_hv_t *self);

#ifdef __cplusplus
}
#endif

#endif // DTL_HV_H_
