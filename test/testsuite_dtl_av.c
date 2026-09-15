/*****************************************************************************
* \file      testsuite_dtl_av.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for DTL array
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
#include "dtl_sv.h"
#include "dtl_av.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_dtl_av_new_delete(CuTest *tc);
static void test_dtl_av_push_pop(CuTest *tc);
static void test_dtl_av_get_set(CuTest *tc);
static void test_dtl_av_sort_i32(CuTest *tc);
static void test_dtl_av_sort_strings(CuTest *tc);
static void test_dtl_av_make(CuTest *tc);
static void test_dtl_av_shift_unshift(CuTest *tc);
static void test_dtl_av_is_empty_clear(CuTest *tc);
static void test_dtl_av_exists(CuTest *tc);
static void test_dtl_av_extend_fill(CuTest *tc);
static void test_dtl_av_sort_reverse(CuTest *tc);
static void test_dtl_av_sort_errors(CuTest *tc);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite *testsuite_dtl_av(void)
{
   CuSuite *suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_av_new_delete);
   SUITE_ADD_TEST(suite, test_dtl_av_push_pop);
   SUITE_ADD_TEST(suite, test_dtl_av_get_set);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_i32);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_strings);
   SUITE_ADD_TEST(suite, test_dtl_av_make);
   SUITE_ADD_TEST(suite, test_dtl_av_shift_unshift);
   SUITE_ADD_TEST(suite, test_dtl_av_is_empty_clear);
   SUITE_ADD_TEST(suite, test_dtl_av_exists);
   SUITE_ADD_TEST(suite, test_dtl_av_extend_fill);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_reverse);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_errors);

   return suite;
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_av_new_delete(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);
   dtl_av_delete(av);
}

static void test_dtl_av_push_pop(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);
   dtl_sv_t *sv = dtl_sv_make_i32(82);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(1), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(2), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(4), false);
   dtl_av_push(av, (dtl_dv_t *) sv, false);
   dtl_inc_ref(sv);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(av);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv);

   // Pop test
   av = dtl_av_new();
   CuAssertPtrEquals(tc, NULL, dtl_av_pop(av));
   dtl_sv_t *item = dtl_sv_make_i32(99);
   dtl_av_push(av, (dtl_dv_t *) item, false);
   CuAssertIntEquals(tc, 1, dtl_av_length(av));
   dtl_dv_t *popped = dtl_av_pop(av);
   CuAssertPtrEquals(tc, item, popped);
   CuAssertIntEquals(tc, 0, dtl_av_length(av));
   dtl_dec_ref(popped);
   dtl_dec_ref(av);
}

static void test_dtl_av_get_set(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);

   dtl_sv_t *sv = dtl_sv_make_i32(1);
   dtl_av_set(av, 3, (dtl_dv_t *) sv);
   CuAssertPtrEquals(tc, &g_dtl_sv_none, *dtl_av_get(av, 0));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, *dtl_av_get(av, 1));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, *dtl_av_get(av, 2));
   dtl_dv_t *dv = *dtl_av_get(av, 3);
   CuAssertPtrEquals(tc, dv, sv);
   CuAssertIntEquals(tc, DTL_DV_SCALAR, (int) dtl_dv_type(dv));
   CuAssertIntEquals(tc, 1, dtl_sv_to_i32((dtl_sv_t *) dv, NULL));

   // Overwrite index 3
   dtl_sv_t *sv2 = dtl_sv_make_i32(2);
   dtl_av_set(av, 3, (dtl_dv_t *) sv2);
   CuAssertPtrEquals(tc, sv2, *dtl_av_get(av, 3));

   dtl_dec_ref(av);
}

static void test_dtl_av_sort_i32(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);

   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(9), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(2), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(5), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(10), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(4), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(7), false);
   CuAssertIntEquals(tc, 6, dtl_av_length(av));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(av, NULL, false));
   CuAssertIntEquals(tc, 2, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 0), NULL));
   CuAssertIntEquals(tc, 4, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 1), NULL));
   CuAssertIntEquals(tc, 5, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 2), NULL));
   CuAssertIntEquals(tc, 7, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 3), NULL));
   CuAssertIntEquals(tc, 9, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 4), NULL));
   CuAssertIntEquals(tc, 10, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 5), NULL));

   dtl_dec_ref(av);
}

static void test_dtl_av_sort_strings(CuTest *tc)
{
   bool ok = false;
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);

   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_cstr("strawberry"), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_cstr("apple"), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_cstr("raspberry"), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_cstr("pear"), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_cstr("pineapple"), false);
   CuAssertIntEquals(tc, 5, dtl_av_length(av));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(av, NULL, false));
   CuAssertStrEquals(tc, "apple", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(av, 0), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "pear", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(av, 1), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "pineapple", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(av, 2), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "raspberry", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(av, 3), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "strawberry", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(av, 4), &ok));
   CuAssertTrue(tc, ok);

   dtl_dec_ref(av);
}

static void test_dtl_av_make(CuTest *tc)
{
   dtl_dv_t *items[3];
   items[0] = (dtl_dv_t *) dtl_sv_make_i32(10);
   items[1] = (dtl_dv_t *) dtl_sv_make_i32(20);
   items[2] = (dtl_dv_t *) dtl_sv_make_i32(30);

   dtl_av_t *av = dtl_av_make(items, 3);
   CuAssertPtrNotNull(tc, av);
   CuAssertIntEquals(tc, 3, dtl_av_length(av));
   CuAssertIntEquals(tc, 10, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 0), NULL));
   CuAssertIntEquals(tc, 20, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 1), NULL));
   CuAssertIntEquals(tc, 30, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 2), NULL));
   dtl_dec_ref(av);

   CuAssertPtrEquals(tc, NULL, dtl_av_make(items, -1));
}

static void test_dtl_av_shift_unshift(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);
   CuAssertPtrEquals(tc, NULL, dtl_av_shift(av));

   dtl_av_unshift(av, (dtl_dv_t *) dtl_sv_make_i32(1));
   dtl_av_unshift(av, (dtl_dv_t *) dtl_sv_make_i32(2));
   CuAssertIntEquals(tc, 2, dtl_av_length(av));
   CuAssertIntEquals(tc, 2, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 0), NULL));
   CuAssertIntEquals(tc, 1, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 1), NULL));

   dtl_dv_t *first = dtl_av_shift(av);
   CuAssertPtrNotNull(tc, first);
   CuAssertIntEquals(tc, 2, dtl_sv_to_i32((dtl_sv_t *) first, NULL));
   dtl_dec_ref(first);
   CuAssertIntEquals(tc, 1, dtl_av_length(av));

   dtl_dec_ref(av);
}

static void test_dtl_av_is_empty_clear(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertTrue(tc, dtl_av_is_empty(av));
   CuAssertIntEquals(tc, 0, dtl_av_length(av));

   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(5), false);
   CuAssertTrue(tc, !dtl_av_is_empty(av));
   CuAssertIntEquals(tc, 1, dtl_av_length(av));

   dtl_av_clear(av);
   CuAssertTrue(tc, dtl_av_is_empty(av));
   CuAssertIntEquals(tc, 0, dtl_av_length(av));

   dtl_dec_ref(av);
}

static void test_dtl_av_exists(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertTrue(tc, !dtl_av_exists(av, 0));
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(100), false);
   CuAssertTrue(tc, dtl_av_exists(av, 0));
   CuAssertTrue(tc, !dtl_av_exists(av, 1));
   CuAssertTrue(tc, dtl_av_exists(av, -1)); // Python-like negative indexing: -1 is last element
   CuAssertTrue(tc, !dtl_av_exists(av, -2));
   dtl_dec_ref(av);
}

static void test_dtl_av_extend_fill(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   dtl_av_extend(av, 10);
   CuAssertIntEquals(tc, 10, dtl_av_length(av));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, dtl_av_value(av, 0));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, dtl_av_value(av, 9));
   dtl_dec_ref(av);

   av = dtl_av_new();
   dtl_av_fill(av, 5);
   CuAssertIntEquals(tc, 5, dtl_av_length(av));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, dtl_av_value(av, 0));
   CuAssertPtrEquals(tc, &g_dtl_sv_none, dtl_av_value(av, 4));
   dtl_dec_ref(av);
}

static void test_dtl_av_sort_reverse(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(3), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(1), false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(2), false);

   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(av, NULL, true));
   CuAssertIntEquals(tc, 3, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 0), NULL));
   CuAssertIntEquals(tc, 2, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 1), NULL));
   CuAssertIntEquals(tc, 1, dtl_sv_to_i32((dtl_sv_t *) dtl_av_value(av, 2), NULL));

   dtl_dec_ref(av);
}

static void test_dtl_av_sort_errors(CuTest *tc)
{
   CuAssertIntEquals(tc, DTL_INVALID_ARGUMENT_ERROR, dtl_av_sort(NULL, NULL, false));

   dtl_av_t *av = dtl_av_new();
   // Non-NULL key function pointer (not implemented)
   CuAssertIntEquals(tc, DTL_NOT_IMPLEMENTED_ERROR, dtl_av_sort(av, (dtl_key_func_t *) 1, false));

   // Sorting array containing non-scalars
   dtl_av_t *nested = dtl_av_new();
   dtl_av_push(av, (dtl_dv_t *) nested, false);
   dtl_av_push(av, (dtl_dv_t *) dtl_sv_make_i32(1), false);
   CuAssertIntEquals(tc, DTL_TYPE_ERROR, dtl_av_sort(av, NULL, false));

   dtl_dec_ref(av);
}
