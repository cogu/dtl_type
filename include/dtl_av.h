/*****************************************************************************
* \file      dtl_av.h
* \author    Conny Gustafsson
* \date      2013-03-10
* \brief     DTL Array Value
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/
#ifndef DTL_AV_H_
#define DTL_AV_H_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include "dtl_dv.h"
#include "adt_ary.h"
#include "dtl_error.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

/**
 * \brief Dynamic Array Value type.
 */
typedef struct dtl_av_tag {
   DTL_DV_HEAD(adt_ary_t)
} dtl_av_t;

/**
 * \brief Key extraction function pointer for sorting arrays.
 */
typedef dtl_dv_t *(dtl_key_func_t)(const dtl_dv_t *dv);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

// Constructor/Destructor

/**
 * \brief Allocates and initializes a new empty array.
 * \return Pointer to new array, or NULL on allocation failure.
 */
dtl_av_t *dtl_av_new(void);

/**
 * \brief Creates an array populated with an initial array of dynamic values.
 * \param values Array of dynamic value pointers.
 * \param len Number of elements in values.
 * \return Pointer to new array, or NULL on failure.
 */
dtl_av_t *dtl_av_make(dtl_dv_t **values, int32_t len);

/**
 * \brief Destroys and frees an array.
 * \param self Pointer to array.
 */
void dtl_av_delete(dtl_av_t *self);

/**
 * \brief Initializes a pre-allocated array.
 * \param self Pointer to array.
 */
void dtl_av_create(dtl_av_t *self);

/**
 * \brief Destroys contents of an array.
 * \param self Pointer to array.
 */
void dtl_av_destroy(dtl_av_t *self);

// Accessors

/**
 * \brief Sets the value at a specific index in the array.
 * \param self Pointer to array.
 * \param index Target index.
 * \param value Dynamic value to store.
 * \return Pointer to element slot, or NULL on failure.
 */
dtl_dv_t **dtl_av_set(dtl_av_t *self, int32_t index, dtl_dv_t *value);

/**
 * \brief Retrieves a pointer to the dynamic value pointer at a specific index.
 * \param self Pointer to array.
 * \param index Target index.
 * \return Pointer to stored dtl_dv_t pointer, or NULL if out of bounds.
 */
dtl_dv_t **dtl_av_get(const dtl_av_t *self, int32_t index);

/**
 * \brief Appends a dynamic value to the end of the array.
 * \param self Pointer to array.
 * \param dv Dynamic value to append.
 * \param auto_increment_ref If true, increments reference count of dv.
 */
void dtl_av_push(dtl_av_t *self, dtl_dv_t *dv, bool auto_increment_ref);

/**
 * \brief Removes and returns the last element of the array.
 * \param self Pointer to array.
 * \return Removed dynamic value, or NULL if array is empty.
 */
dtl_dv_t *dtl_av_pop(dtl_av_t *self);

/**
 * \brief Removes and returns the first element of the array.
 * \param self Pointer to array.
 * \return Removed dynamic value, or NULL if array is empty.
 */
dtl_dv_t *dtl_av_shift(dtl_av_t *self);

/**
 * \brief Inserts a dynamic value at the beginning of the array.
 * \param self Pointer to array.
 * \param value Dynamic value to prepend.
 */
void dtl_av_unshift(dtl_av_t *self, dtl_dv_t *value);

/**
 * \brief Retrieves the dynamic value at a specific index.
 * \param self Pointer to array.
 * \param index Target index.
 * \return Stored dynamic value, or NULL if index out of bounds.
 */
dtl_dv_t *dtl_av_value(const dtl_av_t *self, int32_t index);

// Utility functions

/**
 * \brief Extends array capacity to at least len elements.
 * \param self Pointer to array.
 * \param len Target capacity.
 */
void dtl_av_extend(dtl_av_t *self, int32_t len);

/**
 * \brief Fills array with default empty elements up to len elements.
 * \param self Pointer to array.
 * \param len Target length.
 */
void dtl_av_fill(dtl_av_t *self, int32_t len);

/**
 * \brief Clears all elements from array.
 * \param self Pointer to array.
 */
void dtl_av_clear(dtl_av_t *self);

/**
 * \brief Returns the number of elements in the array.
 * \param self Pointer to array.
 * \return Number of elements, or -1 if self is NULL.
 */
int32_t dtl_av_length(const dtl_av_t *self);

/**
 * \brief Checks if the array is empty.
 * \param self Pointer to array.
 * \return true if empty or self is NULL, false otherwise.
 */
bool dtl_av_is_empty(const dtl_av_t *self);

/**
 * \brief Checks if an element exists at a specific index.
 * \param self Pointer to array.
 * \param index Target index.
 * \return true if index exists, false otherwise.
 */
bool dtl_av_exists(const dtl_av_t *self, int32_t index);

/**
 * \brief Sorts scalar elements in the array using insertion sort.
 * \param self Pointer to array.
 * \param key Optional key extraction function (currently must be NULL).
 * \param reverse If true, sorts in descending order; otherwise ascending.
 * \return DTL_NO_ERROR on success, or error code.
 */
dtl_error_t dtl_av_sort(dtl_av_t *self, dtl_key_func_t *key, bool reverse);

#ifdef __cplusplus
}
#endif

#endif // DTL_AV_H_
