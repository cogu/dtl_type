/*****************************************************************************
* \file      dtl_sv.h
* \author    Conny Gustafsson
* \date      2013-03-07
* \brief     DTL Scalar Value
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/
#ifndef DTL_SV_H_
#define DTL_SV_H_

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdbool.h>
#include <stdint.h>
#include "dtl_dv.h"
#include "adt_str.h"
#include "adt_bytes.h"
#include "adt_bytearray.h"
#include "dtl_error.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////
#define DTL_SV_TYPE_MASK      0xF0u
#define DTL_SV_TYPE_SHIFT     4u

/**
 * \brief Custom pointer wrapper with destructor callback.
 */
typedef struct dtl_pv_tag {
   void *p;
   void (*destructor)(void *);
} dtl_pv_t;

/**
 * \brief Scalar value storage union.
 */
typedef union dtl_sv_value_tag {
   int32_t         i32;
   uint32_t        u32;
   int64_t         i64;
   uint64_t        u64;
   float           flt;
   double          dbl;
   char            cr;
   adt_str_t      *str;
   dtl_pv_t        ptr;
   dtl_dv_t       *dv;
   bool            bl;
   adt_bytes_t    *bytes;
   adt_bytearray_t *bytearray;
} dtl_sv_value_t;

/**
 * \brief Internal scalar extension structure.
 */
typedef struct dtl_svx_tag {
   adt_str_t *tmp_str;
   dtl_sv_value_t val;
} dtl_svx_t;

/**
 * \brief Dynamic Scalar Value type.
 */
typedef struct dtl_sv_tag {
   DTL_DV_HEAD(dtl_svx_t)
} dtl_sv_t;

/**
 * \brief Scalar type identifier.
 */
typedef enum dtl_sv_type_tag {
   DTL_SV_NONE = 0,
   DTL_SV_I32,
   DTL_SV_U32,
   DTL_SV_I64,
   DTL_SV_U64,
   DTL_SV_FLT,
   DTL_SV_DBL,
   DTL_SV_CHAR,
   DTL_SV_BOOL,
   DTL_SV_STR,
   DTL_SV_PTR,
   DTL_SV_DV,
   DTL_SV_BYTES,
   DTL_SV_BYTEARRAY
} dtl_sv_type_id;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC VARIABLES
//////////////////////////////////////////////////////////////////////////////
extern dtl_sv_t g_dtl_sv_none;

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////

// Forward declarations
struct dtl_av_tag;
struct dtl_hv_tag;

// Constructor/Destructor

/**
 * \brief Allocates and initializes a new scalar value (type DTL_SV_NONE).
 * \return Pointer to new scalar value, or NULL on failure.
 */
dtl_sv_t *dtl_sv_new(void);

/**
 * \brief Destroys and frees a scalar value.
 * \param self Pointer to scalar value.
 */
void dtl_sv_delete(dtl_sv_t *self);

/**
 * \brief Initializes a pre-allocated scalar value.
 * \param self Pointer to scalar value.
 */
void dtl_sv_create(dtl_sv_t *self);

/**
 * \brief Frees internal resources of a scalar value.
 * \param self Pointer to scalar value.
 */
void dtl_sv_destroy(dtl_sv_t *self);

/**
 * \brief Creates a scalar holding an int32_t value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_i32(int32_t value);

/**
 * \brief Creates a scalar holding a uint32_t value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_u32(uint32_t value);

/**
 * \brief Creates a scalar holding an int64_t value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_i64(int64_t value);

/**
 * \brief Creates a scalar holding a uint64_t value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_u64(uint64_t value);

/**
 * \brief Creates a scalar holding a float value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_flt(float value);

/**
 * \brief Creates a scalar holding a double value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_dbl(double value);

/**
 * \brief Creates a scalar holding a bool value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_bool(bool value);

/**
 * \brief Creates a scalar holding a char value.
 * \param value Value to initialize scalar with.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_char(char value);

/**
 * \brief Creates a scalar holding a generic pointer with an optional destructor.
 * \param ptr Pointer to manage.
 * \param destructor Function called to free ptr upon scalar destruction, or NULL.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_ptr(void *ptr, void (*destructor)(void *));

/**
 * \brief Creates a scalar holding a copy of an adt_str_t string.
 * \param str String to clone.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_str(const adt_str_t *str);

/**
 * \brief Creates a scalar holding a C string.
 * \param str C string to copy.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_cstr(const char *str);

/**
 * \brief Creates a scalar wrapping a dynamic value.
 * \param dv Dynamic value to wrap.
 * \param auto_inc_ref If true, increments reference count of dv.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_dv(dtl_dv_t *dv, bool auto_inc_ref);

/**
 * \brief Creates a scalar taking ownership of an adt_bytes_t object.
 * \param bytes Byte buffer object.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_bytes(adt_bytes_t *bytes);

/**
 * \brief Creates a scalar copying raw byte data into an internal adt_bytes_t.
 * \param data_buf Pointer to source bytes.
 * \param data_len Number of bytes.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_bytes_raw(const uint8_t *data_buf, uint32_t data_len);

/**
 * \brief Creates a scalar taking ownership of an adt_bytearray_t object.
 * \param array Byte array object.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_bytearray(adt_bytearray_t *array);

/**
 * \brief Creates a scalar copying raw byte data into an internal adt_bytearray_t.
 * \param data_buf Pointer to source bytes.
 * \param data_len Number of bytes.
 * \return Pointer to new scalar, or NULL on failure.
 */
dtl_sv_t *dtl_sv_make_bytearray_raw(const uint8_t *data_buf, uint32_t data_len);

// Getters

/**
 * \brief Returns the scalar type ID.
 * \param self Pointer to scalar value.
 * \return Scalar type ID.
 */
dtl_sv_type_id dtl_sv_type(const dtl_sv_t *self);

/**
 * \brief Returns the underlying dynamic value type ID if scalar wraps a dv.
 * \param self Pointer to scalar value.
 * \return Dynamic value type ID, or DTL_DV_NULL if self is not wrapping a dv.
 */
dtl_dv_type_id dtl_sv_dv_type(const dtl_sv_t *self);

/**
 * \brief Gets a read-only pointer to the managed adt_bytes_t object.
 * \param self Pointer to scalar value.
 * \return Weak pointer to bytes object, or NULL if type is not DTL_SV_BYTES.
 */
const adt_bytes_t *dtl_sv_get_bytes(const dtl_sv_t *self);

/**
 * \brief Gets a read-only pointer to the managed adt_bytearray_t object.
 * \param self Pointer to scalar value.
 * \return Weak pointer to bytearray object, or NULL if type is not DTL_SV_BYTEARRAY.
 */
const adt_bytearray_t *dtl_sv_get_bytearray(const dtl_sv_t *self);

// Setters

/**
 * \brief Sets scalar value to an int32_t.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_i32(dtl_sv_t *self, int32_t value);

/**
 * \brief Sets scalar value to a uint32_t.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_u32(dtl_sv_t *self, uint32_t value);

/**
 * \brief Sets scalar value to an int64_t.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_i64(dtl_sv_t *self, int64_t value);

/**
 * \brief Sets scalar value to a uint64_t.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_u64(dtl_sv_t *self, uint64_t value);

/**
 * \brief Sets scalar value to a float.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_flt(dtl_sv_t *self, float value);

/**
 * \brief Sets scalar value to a double.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_dbl(dtl_sv_t *self, double value);

/**
 * \brief Sets scalar value to a boolean.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_bool(dtl_sv_t *self, bool value);

/**
 * \brief Sets scalar value to a char.
 * \param self Pointer to scalar value.
 * \param value Value to set.
 */
void dtl_sv_set_char(dtl_sv_t *self, char value);

/**
 * \brief Sets scalar value to a managed pointer with optional destructor.
 * \param self Pointer to scalar value.
 * \param ptr Pointer to manage.
 * \param destructor Function called on destruction, or NULL.
 */
void dtl_sv_set_ptr(dtl_sv_t *self, void *ptr, void (*destructor)(void *));

/**
 * \brief Sets scalar value to a copy of an adt_str_t.
 * \param self Pointer to scalar value.
 * \param str String to copy.
 */
void dtl_sv_set_str(dtl_sv_t *self, const adt_str_t *str);

/**
 * \brief Sets scalar value to a C string.
 * \param self Pointer to scalar value.
 * \param str C string to copy.
 */
void dtl_sv_set_cstr(dtl_sv_t *self, const char *str);

/**
 * \brief Sets scalar value from a bounded byte buffer slice.
 * \param self Pointer to scalar value.
 * \param begin Pointer to start of slice.
 * \param end Pointer to end of slice.
 */
void dtl_sv_set_bstr(dtl_sv_t *self, const uint8_t *begin, const uint8_t *end);

/**
 * \brief Sets scalar value to wrap a dynamic value.
 * \param self Pointer to scalar value.
 * \param dv Dynamic value to wrap.
 * \param auto_inc_ref If true, increments reference count of dv.
 */
void dtl_sv_set_dv(dtl_sv_t *self, dtl_dv_t *dv, bool auto_inc_ref);

/**
 * \brief Sets scalar value taking ownership of an adt_bytes_t object.
 * \param self Pointer to scalar value.
 * \param bytes Byte buffer object.
 */
void dtl_sv_set_bytes(dtl_sv_t *self, adt_bytes_t *bytes);

/**
 * \brief Sets scalar value by copying raw byte buffer into an internal adt_bytes_t.
 * \param self Pointer to scalar value.
 * \param data_buf Pointer to source bytes.
 * \param data_len Number of bytes.
 */
void dtl_sv_set_bytes_raw(dtl_sv_t *self, const uint8_t *data_buf, uint32_t data_len);

/**
 * \brief Sets scalar value taking ownership of an adt_bytearray_t object.
 * \param self Pointer to scalar value.
 * \param array Byte array object.
 */
void dtl_sv_set_bytearray(dtl_sv_t *self, adt_bytearray_t *array);

/**
 * \brief Sets scalar value by copying raw byte buffer into an internal adt_bytearray_t.
 * \param self Pointer to scalar value.
 * \param data_buf Pointer to source bytes.
 * \param data_len Number of bytes.
 */
void dtl_sv_set_bytearray_raw(dtl_sv_t *self, const uint8_t *data_buf, uint32_t data_len);

/**
 * \brief Assigns ownership of an adt_bytes_t object to the scalar value.
 * \param self Pointer to scalar value.
 * \param bytes Byte buffer object.
 */
void dtl_sv_take_bytes(dtl_sv_t *self, adt_bytes_t *bytes);

// Conversion functions

/**
 * \brief Converts scalar value to an int32_t.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted int32_t, or 0 on conversion failure.
 */
int32_t dtl_sv_to_i32(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a uint32_t.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted uint32_t, or 0 on conversion failure.
 */
uint32_t dtl_sv_to_u32(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to an int64_t.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted int64_t, or 0 on conversion failure.
 */
int64_t dtl_sv_to_i64(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a uint64_t.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted uint64_t, or 0 on conversion failure.
 */
uint64_t dtl_sv_to_u64(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a float.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted float, or 0.0f on conversion failure.
 */
float dtl_sv_to_flt(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a double.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted double, or 0.0 on conversion failure.
 */
double dtl_sv_to_dbl(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a boolean.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted boolean, or false on conversion failure.
 */
bool dtl_sv_to_bool(const dtl_sv_t *self, bool *ok);

/**
 * \brief Converts scalar value to a char.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Converted char, or '\0' on conversion failure.
 */
char dtl_sv_to_char(const dtl_sv_t *self, bool *ok);

/**
 * \brief Retrieves the raw pointer from a DTL_SV_PTR or DTL_SV_DV scalar.
 * \param self Pointer to scalar value.
 * \return Pointer value, or NULL if not a pointer type.
 */
void *dtl_sv_to_ptr(const dtl_sv_t *self);

/**
 * \brief Clones scalar value into a new adt_str_t string representation.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Pointer to newly allocated adt_str_t, or NULL on failure.
 */
adt_str_t *dtl_sv_to_str(const dtl_sv_t *self, bool *ok);

/**
 * \brief Returns C string representation stored in temporary internal buffer.
 * \param self Pointer to scalar value.
 * \param ok Optional pointer receiving true if conversion succeeded, false otherwise.
 * \return Pointer to C string, or NULL on failure.
 */
const char *dtl_sv_to_cstr(dtl_sv_t *self, bool *ok);

/**
 * \brief Unwraps dynamic value if scalar type is DTL_SV_DV.
 * \param self Pointer to scalar value.
 * \return Pointer to underlying dtl_dv_t, or NULL.
 */
dtl_dv_t *dtl_sv_to_dv(const dtl_sv_t *self);

/**
 * \brief Returns self if type is DTL_SV_DV and underlying DV is scalar, or returns underlying scalar.
 * \param self Pointer to scalar value.
 * \return Pointer to scalar value, or NULL.
 */
dtl_sv_t *dtl_sv_to_sv(const dtl_sv_t *self);

/**
 * \brief Unwraps array value if scalar type is DTL_SV_DV wrapping an array.
 * \param self Pointer to scalar value.
 * \return Pointer to array value, or NULL.
 */
struct dtl_av_tag *dtl_sv_to_av(const dtl_sv_t *self);

/**
 * \brief Unwraps hash value if scalar type is DTL_SV_DV wrapping a hash.
 * \param self Pointer to scalar value.
 * \return Pointer to hash value, or NULL.
 */
struct dtl_hv_tag *dtl_sv_to_hv(const dtl_sv_t *self);

// Comparison functions

/**
 * \brief Performs less-than comparison between two scalars.
 * \param self Pointer to left scalar.
 * \param other Pointer to right scalar.
 * \param result Pointer receiving comparison outcome (true if self < other).
 * \return DTL_NO_ERROR on success, or error code on type mismatch / invalid argument.
 */
dtl_error_t dtl_sv_lt(const dtl_sv_t *self, const dtl_sv_t *other, bool *result);

// Macros
#define dtl_sv_none() (&g_dtl_sv_none)

#ifdef __cplusplus
}
#endif

#endif // DTL_SV_H_
