/*****************************************************************************
* \file      dtl_error.h
* \author    Conny Gustafsson
* \date      2019-07-28
* \brief     DTL error codes
*
* Copyright (c) 2019-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/
#ifndef DTL_ERROR_H_
#define DTL_ERROR_H_

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

/**
 * \brief Error code type returned by DTL functions.
 */
typedef int32_t dtl_error_t;

#define DTL_NO_ERROR                 ((dtl_error_t) 0) /**< Operation completed successfully */
#define DTL_INVALID_ARGUMENT_ERROR   ((dtl_error_t) 1) /**< Invalid argument provided */
#define DTL_MEM_ERROR                ((dtl_error_t) 2) /**< Memory allocation failure */
#define DTL_NOT_IMPLEMENTED_ERROR    ((dtl_error_t) 3) /**< Feature or method not implemented */
#define DTL_TYPE_ERROR               ((dtl_error_t) 4) /**< Type mismatch error */
#define DTL_CONVERSION_ERROR         ((dtl_error_t) 5) /**< Type conversion error */

#ifdef __cplusplus
}
#endif

#endif // DTL_ERROR_H_
