/*****************************************************************************
* \file      testsuite_dtl_dv.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for dtl_dv
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CuTest.h"
#include "dtl_dv.h"
#include "dtl_sv.h"
#include "dtl_av.h"
#include "dtl_hv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_dtl_dv_null(CuTest *tc);
static void test_dtl_dv_inc_dec_ref(CuTest *tc);
static void test_dtl_dv_type(CuTest *tc);
static void test_dtl_dv_vdelete(CuTest *tc);
static void test_dtl_dv_dec_ref_void(CuTest *tc);
static void test_dtl_dv_none_immunity(CuTest *tc);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite *testsuite_dtl_dv(void)
{
   CuSuite *suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_dv_null);
   SUITE_ADD_TEST(suite, test_dtl_dv_inc_dec_ref);
   SUITE_ADD_TEST(suite, test_dtl_dv_type);
   SUITE_ADD_TEST(suite, test_dtl_dv_vdelete);
   SUITE_ADD_TEST(suite, test_dtl_dv_dec_ref_void);
   SUITE_ADD_TEST(suite, test_dtl_dv_none_immunity);

   return suite;
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_dv_null(CuTest *tc)
{
   dtl_dv_t *dv = dtl_dv_null();
   CuAssertPtrNotNull(tc, dv);
   CuAssertIntEquals(tc, DTL_DV_NULL, (int) dtl_dv_type(dv));
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(dv));
   dtl_dec_ref(dv);
}

static void test_dtl_dv_inc_dec_ref(CuTest *tc)
{
   // Scalar
   dtl_sv_t *sv = dtl_sv_make_i32(42);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_inc_ref(sv);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv);

   // Array
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(av));
   dtl_inc_ref(av);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(av));
   dtl_dec_ref(av);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(av));
   dtl_dec_ref(av);

   // Hash
   dtl_hv_t *hv = dtl_hv_new();
   CuAssertPtrNotNull(tc, hv);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(hv));
   dtl_inc_ref(hv);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(hv));
   dtl_dec_ref(hv);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(hv));
   dtl_dec_ref(hv);
}

static void test_dtl_dv_type(CuTest *tc)
{
   CuAssertIntEquals(tc, DTL_DV_NULL, (int) dtl_dv_type(NULL));

   dtl_dv_t *null_dv = dtl_dv_null();
   CuAssertIntEquals(tc, DTL_DV_NULL, (int) dtl_dv_type(null_dv));
   dtl_dec_ref(null_dv);

   dtl_sv_t *sv = dtl_sv_make_i32(10);
   CuAssertIntEquals(tc, DTL_DV_SCALAR, (int) dtl_dv_type((dtl_dv_t *) sv));
   dtl_dec_ref(sv);

   dtl_av_t *av = dtl_av_new();
   CuAssertIntEquals(tc, DTL_DV_ARRAY, (int) dtl_dv_type((dtl_dv_t *) av));
   dtl_dec_ref(av);

   dtl_hv_t *hv = dtl_hv_new();
   CuAssertIntEquals(tc, DTL_DV_HASH, (int) dtl_dv_type((dtl_dv_t *) hv));
   dtl_dec_ref(hv);
}

static void test_dtl_dv_vdelete(CuTest *tc)
{
   dtl_sv_t *sv = dtl_sv_make_cstr("vdelete test");
   CuAssertPtrNotNull(tc, sv);
   dtl_dv_vdelete(sv);

   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);
   dtl_dv_vdelete(av);

   dtl_hv_t *hv = dtl_hv_new();
   CuAssertPtrNotNull(tc, hv);
   dtl_dv_vdelete(hv);

   dtl_dv_t *null_dv = dtl_dv_null();
   CuAssertPtrNotNull(tc, null_dv);
   dtl_dv_vdelete(null_dv);
}

static void test_dtl_dv_dec_ref_void(CuTest *tc)
{
   dtl_sv_t *sv = dtl_sv_make_i32(100);
   CuAssertPtrNotNull(tc, sv);
   dtl_inc_ref(sv);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(sv));
   dtl_dv_dec_ref_void(sv);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_dv_dec_ref_void(sv);
}

static void test_dtl_dv_none_immunity(CuTest *tc)
{
   dtl_sv_t *none = dtl_sv_none();
   CuAssertPtrNotNull(tc, none);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(none));
   dtl_dec_ref(none);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(none));
}
