/*****************************************************************************
* \file      testsuite_dtl_hv.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for DTL hash
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
#include "dtl_hv.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_dtl_hv_new_delete(CuTest *tc);
static void test_dtl_hv_get_cstr_set(CuTest *tc);
static void test_dtl_hv_keys_sorted(CuTest *tc);
static void test_dtl_hv_iter(CuTest *tc);
static void test_dtl_hv_remove_cstr(CuTest *tc);
static void test_dtl_hv_exists_cstr(CuTest *tc);
static void test_dtl_hv_set_overwrite(CuTest *tc);
static void test_dtl_hv_auto_inc_ref(CuTest *tc);
static void test_dtl_hv_null_safety(CuTest *tc);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite *testsuite_dtl_hv(void)
{
   CuSuite *suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_hv_new_delete);
   SUITE_ADD_TEST(suite, test_dtl_hv_get_cstr_set);
   SUITE_ADD_TEST(suite, test_dtl_hv_keys_sorted);
   SUITE_ADD_TEST(suite, test_dtl_hv_iter);
   SUITE_ADD_TEST(suite, test_dtl_hv_remove_cstr);
   SUITE_ADD_TEST(suite, test_dtl_hv_exists_cstr);
   SUITE_ADD_TEST(suite, test_dtl_hv_set_overwrite);
   SUITE_ADD_TEST(suite, test_dtl_hv_auto_inc_ref);
   SUITE_ADD_TEST(suite, test_dtl_hv_null_safety);

   return suite;
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_hv_new_delete(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   CuAssertPtrNotNull(tc, hv);
   dtl_hv_delete(hv);
}

static void test_dtl_hv_get_cstr_set(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   CuAssertPtrNotNull(tc, hv);
   dtl_sv_t *sv = dtl_sv_make_i32(82);
   dtl_hv_set_cstr(hv, "First", (dtl_dv_t *) dtl_sv_make_i32(1), false);
   dtl_hv_set_cstr(hv, "Second", (dtl_dv_t *) dtl_sv_make_i32(2), false);
   dtl_hv_set_cstr(hv, "Third", (dtl_dv_t *) dtl_sv_make_i32(4), false);
   dtl_hv_set_cstr(hv, "Fourth", (dtl_dv_t *) sv, false);
   dtl_inc_ref(sv);
   CuAssertIntEquals(tc, 4, (int) dtl_hv_length(hv));
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(sv));

   dtl_dv_t *first = dtl_hv_get_cstr(hv, "First");
   dtl_dv_t *second = dtl_hv_get_cstr(hv, "Second");
   dtl_dv_t *third = dtl_hv_get_cstr(hv, "Third");
   dtl_dv_t *fourth = dtl_hv_get_cstr(hv, "Fourth");
   CuAssertPtrNotNull(tc, first);
   CuAssertPtrNotNull(tc, second);
   CuAssertPtrNotNull(tc, third);
   CuAssertPtrEquals(tc, sv, fourth);
   dtl_dec_ref(hv);
   CuAssertIntEquals(tc, 82, dtl_sv_to_i32((dtl_sv_t *) fourth, NULL));
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv);
}

static void test_dtl_hv_keys_sorted(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_av_t *keys = NULL;
   bool ok = false;
   CuAssertPtrNotNull(tc, hv);

   dtl_hv_set_cstr(hv, "Illinois", (dtl_dv_t *) dtl_sv_make_dbl(3.85), false);
   dtl_hv_set_cstr(hv, "Pennsylvania", (dtl_dv_t *) dtl_sv_make_dbl(3.87), false);
   dtl_hv_set_cstr(hv, "Florida", (dtl_dv_t *) dtl_sv_make_dbl(6.44), false);
   dtl_hv_set_cstr(hv, "Ohio", (dtl_dv_t *) dtl_sv_make_dbl(3.53), false);
   dtl_hv_set_cstr(hv, "California", (dtl_dv_t *) dtl_sv_make_dbl(11.96), false);
   dtl_hv_set_cstr(hv, "Texas", (dtl_dv_t *) dtl_sv_make_dbl(8.68), false);
   CuAssertIntEquals(tc, 6, (int) dtl_hv_length(hv));

   keys = dtl_hv_keys(hv);
   CuAssertPtrNotNull(tc, keys);
   CuAssertIntEquals(tc, 6, dtl_av_length(keys));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(keys, NULL, false));
   CuAssertStrEquals(tc, "California", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 0), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "Florida", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 1), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "Illinois", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 2), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "Ohio", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 3), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "Pennsylvania", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 4), &ok));
   CuAssertTrue(tc, ok);
   CuAssertStrEquals(tc, "Texas", dtl_sv_to_cstr((dtl_sv_t *) dtl_av_value(keys, 5), &ok));
   CuAssertTrue(tc, ok);

   dtl_dec_ref(keys);
   dtl_dec_ref(hv);
}

static void test_dtl_hv_iter(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_dv_t *dv = NULL;
   const char *key = NULL;

   CuAssertPtrNotNull(tc, hv);

   dtl_hv_set_cstr(hv, "First", (dtl_dv_t *) dtl_sv_make_i32(1), false);
   dtl_hv_set_cstr(hv, "Second", (dtl_dv_t *) dtl_sv_make_i32(2), false);
   dtl_hv_set_cstr(hv, "Third", (dtl_dv_t *) dtl_sv_make_i32(4), false);

   CuAssertIntEquals(tc, 3, (int) dtl_hv_length(hv));
   dtl_hv_iter_init(hv);
   dv = dtl_hv_iter_next_cstr(hv, &key);
   CuAssertStrEquals(tc, "First", key);
   CuAssertPtrEquals(tc, dtl_hv_get_cstr(hv, "First"), dv);
   dv = dtl_hv_iter_next_cstr(hv, &key);
   CuAssertStrEquals(tc, "Second", key);
   CuAssertPtrEquals(tc, dtl_hv_get_cstr(hv, "Second"), dv);
   dv = dtl_hv_iter_next_cstr(hv, &key);
   CuAssertStrEquals(tc, "Third", key);
   CuAssertPtrEquals(tc, dtl_hv_get_cstr(hv, "Third"), dv);
   dv = dtl_hv_iter_next_cstr(hv, &key);
   CuAssertPtrEquals(tc, NULL, dv);

   dtl_dec_ref(hv);
}

static void test_dtl_hv_remove_cstr(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_sv_t *val = dtl_sv_make_cstr("to be removed");
   dtl_hv_set_cstr(hv, "key1", (dtl_dv_t *) val, false);
   CuAssertIntEquals(tc, 1, (int) dtl_hv_length(hv));

   dtl_dv_t *removed = dtl_hv_remove_cstr(hv, "key1");
   CuAssertPtrEquals(tc, val, removed);
   CuAssertIntEquals(tc, 0, (int) dtl_hv_length(hv));
   CuAssertPtrEquals(tc, NULL, dtl_hv_get_cstr(hv, "key1"));
   dtl_dec_ref(removed);

   // Non-existent remove returns NULL
   CuAssertPtrEquals(tc, NULL, dtl_hv_remove_cstr(hv, "nonexistent"));

   dtl_dec_ref(hv);
}

static void test_dtl_hv_exists_cstr(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   CuAssertTrue(tc, !dtl_hv_exists_cstr(hv, "item"));
   dtl_hv_set_cstr(hv, "item", (dtl_dv_t *) dtl_sv_make_i32(10), false);
   CuAssertTrue(tc, dtl_hv_exists_cstr(hv, "item"));
   dtl_dv_t *removed = dtl_hv_remove_cstr(hv, "item");
   CuAssertTrue(tc, !dtl_hv_exists_cstr(hv, "item"));
   dtl_dec_ref(removed);
   dtl_dec_ref(hv);
}

static void test_dtl_hv_set_overwrite(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_sv_t *val1 = dtl_sv_make_i32(1);
   dtl_sv_t *val2 = dtl_sv_make_i32(2);

   dtl_hv_set_cstr(hv, "my_key", (dtl_dv_t *) val1, true);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(val1));

   // Overwrite same key with val2
   dtl_hv_set_cstr(hv, "my_key", (dtl_dv_t *) val2, true);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(val1));
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(val2));
   CuAssertPtrEquals(tc, val2, dtl_hv_get_cstr(hv, "my_key"));

   dtl_dec_ref(val1);
   dtl_dec_ref(val2);
   dtl_dec_ref(hv);
}

static void test_dtl_hv_auto_inc_ref(CuTest *tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_sv_t *val = dtl_sv_make_i32(99);

   // auto_increment_ref = true
   dtl_hv_set_cstr(hv, "k1", (dtl_dv_t *) val, true);
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(val));

   // auto_increment_ref = false
   dtl_sv_t *val2 = dtl_sv_make_i32(100);
   dtl_hv_set_cstr(hv, "k2", (dtl_dv_t *) val2, false);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(val2));

   dtl_dec_ref(val);
   dtl_dec_ref(hv);
}

static void test_dtl_hv_null_safety(CuTest *tc)
{
   CuAssertIntEquals(tc, 0, (int) dtl_hv_length(NULL));
   CuAssertTrue(tc, !dtl_hv_exists_cstr(NULL, "key"));
   CuAssertPtrEquals(tc, NULL, dtl_hv_get_cstr(NULL, "key"));
   CuAssertPtrEquals(tc, NULL, dtl_hv_remove_cstr(NULL, "key"));
   CuAssertPtrEquals(tc, NULL, dtl_hv_keys(NULL));
}
