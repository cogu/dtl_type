/*****************************************************************************
* \file      testsuite_dtl_sv.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for dtl_sv
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
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_dtl_sv_create(CuTest *tc);
static void test_dtl_sv_make(CuTest *tc);
static void test_dtl_sv_bool(CuTest *tc);
static void test_dtl_sv_lt_i32(CuTest *tc);
static void test_dtl_sv_lt_str(CuTest *tc);
static void test_dtl_sv_make_set_str(CuTest *tc);
static void test_dtl_sv_numeric_setters(CuTest *tc);
static void test_dtl_sv_ptr_destructor(CuTest *tc);
static void test_dtl_sv_dv_wrapper(CuTest *tc);
static void test_dtl_sv_conversions(CuTest *tc);
static void test_dtl_sv_lt_comparisons(CuTest *tc);
static void test_dtl_sv_take_bytes(CuTest *tc);

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite *testsuite_dtl_sv(void)
{
   CuSuite *suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_sv_create);
   SUITE_ADD_TEST(suite, test_dtl_sv_make);
   SUITE_ADD_TEST(suite, test_dtl_sv_bool);
   SUITE_ADD_TEST(suite, test_dtl_sv_lt_i32);
   SUITE_ADD_TEST(suite, test_dtl_sv_lt_str);
   SUITE_ADD_TEST(suite, test_dtl_sv_make_set_str);
   SUITE_ADD_TEST(suite, test_dtl_sv_numeric_setters);
   SUITE_ADD_TEST(suite, test_dtl_sv_ptr_destructor);
   SUITE_ADD_TEST(suite, test_dtl_sv_dv_wrapper);
   SUITE_ADD_TEST(suite, test_dtl_sv_conversions);
   SUITE_ADD_TEST(suite, test_dtl_sv_lt_comparisons);
   SUITE_ADD_TEST(suite, test_dtl_sv_take_bytes);

   return suite;
}

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_sv_create(CuTest *tc)
{
   dtl_sv_t *sv = dtl_sv_new();
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_NONE, (int) dtl_sv_type(sv));
   dtl_sv_delete(sv);
}

static void test_dtl_sv_make(CuTest *tc)
{
   dtl_sv_t *sv;
   adt_bytes_t *bytes1;
   const adt_bytes_t *bytes2;
   adt_bytearray_t *array1;
   const adt_bytearray_t *array2;
   const uint8_t u8_data[5] = {39, 86, 14, 9, 24};

   // int32_t
   sv = dtl_sv_make_i32(124);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_I32, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, 124, dtl_sv_to_i32(sv, NULL));
   dtl_dec_ref(sv);

   // uint32_t
   sv = dtl_sv_make_u32(8328);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_U32, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, 8328, (int) dtl_sv_to_u32(sv, NULL));
   dtl_dec_ref(sv);

   // int64_t
   sv = dtl_sv_make_i64(-1375713549903LL);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_I64, (int) dtl_sv_type(sv));
   CuAssertTrue(tc, -1375713549903LL == dtl_sv_to_i64(sv, NULL));
   dtl_dec_ref(sv);

   // uint64_t
   sv = dtl_sv_make_u64(1375713549903ULL);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_U64, (int) dtl_sv_type(sv));
   CuAssertTrue(tc, 1375713549903ULL == dtl_sv_to_u64(sv, NULL));
   dtl_dec_ref(sv);

   // flt
   sv = dtl_sv_make_flt(64.0f);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_FLT, (int) dtl_sv_type(sv));
   CuAssertDblEquals(tc, 64.0, (double) dtl_sv_to_flt(sv, NULL), 0.001);
   dtl_dec_ref(sv);

   // dbl
   sv = dtl_sv_make_dbl(83.0);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_DBL, (int) dtl_sv_type(sv));
   CuAssertDblEquals(tc, 83.0, dtl_sv_to_dbl(sv, NULL), 0.001);
   dtl_dec_ref(sv);

   // ptr
   int i = 825;
   sv = dtl_sv_make_ptr(&i, NULL);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_PTR, (int) dtl_sv_type(sv));
   CuAssertPtrEquals(tc, &i, dtl_sv_to_ptr(sv));
   dtl_dec_ref(sv);

   // dv
   sv = dtl_sv_make_i32(0);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_I32, (int) dtl_sv_type(sv));
   dtl_sv_t *sv2 = dtl_sv_make_dv((dtl_dv_t *) sv, true);
   CuAssertPtrNotNull(tc, sv2);
   CuAssertIntEquals(tc, DTL_SV_DV, (int) dtl_sv_type(sv2));
   CuAssertPtrEquals(tc, sv, dtl_sv_to_sv(sv2));
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv2));
   CuAssertIntEquals(tc, 2, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv2);
   CuAssertIntEquals(tc, 1, (int) dtl_ref_cnt(sv));
   dtl_dec_ref(sv);

   // bytes
   bytes1 = adt_bytes_new(&u8_data[0], sizeof(u8_data));
   sv = dtl_sv_make_bytes(bytes1);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_BYTES, (int) dtl_sv_type(sv));
   bytes2 = dtl_sv_get_bytes(sv);
   CuAssertPtrNotNull(tc, bytes2);
   CuAssertTrue(tc, adt_bytes_equals(bytes1, bytes2));
   dtl_dec_ref(sv);
   adt_bytes_delete(bytes1);

   // bytes_raw
   sv = dtl_sv_make_bytes_raw(u8_data, (uint32_t) sizeof(u8_data));
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_BYTES, (int) dtl_sv_type(sv));
   bytes2 = dtl_sv_get_bytes(sv);
   CuAssertPtrNotNull(tc, bytes2);
   CuAssertUIntEquals(tc, (uint32_t) sizeof(u8_data), adt_bytes_length(bytes2));
   CuAssertIntEquals(tc, 0, memcmp(u8_data, adt_bytes_const_data(bytes2), sizeof(u8_data)));
   dtl_dec_ref(sv);

   // bytearray
   array1 = adt_bytearray_make(&u8_data[0], sizeof(u8_data));
   sv = dtl_sv_make_bytearray(array1);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_BYTEARRAY, (int) dtl_sv_type(sv));
   array2 = dtl_sv_get_bytearray(sv);
   CuAssertPtrNotNull(tc, array2);
   CuAssertTrue(tc, adt_bytearray_equals(array1, array2));
   dtl_dec_ref(sv);
   adt_bytearray_delete(array1);

   // bytearray_raw
   sv = dtl_sv_make_bytearray_raw(u8_data, (uint32_t) sizeof(u8_data));
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_BYTEARRAY, (int) dtl_sv_type(sv));
   array2 = dtl_sv_get_bytearray(sv);
   CuAssertPtrNotNull(tc, array2);
   CuAssertUIntEquals(tc, (uint32_t) sizeof(u8_data), adt_bytearray_length(array2));
   CuAssertIntEquals(tc, 0, memcmp(u8_data, adt_bytearray_data(array2), sizeof(u8_data)));
   dtl_dec_ref(sv);

   // char
   sv = dtl_sv_make_char('a');
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_CHAR, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, 'a', dtl_sv_to_char(sv, NULL));
   dtl_dec_ref(sv);
}

static void test_dtl_sv_bool(CuTest *tc)
{
   dtl_sv_t *sv;
   bool ok = false;

   sv = dtl_sv_new();
   CuAssertPtrNotNull(tc, sv);
   dtl_sv_set_bool(sv, false);
   CuAssertIntEquals(tc, DTL_SV_BOOL, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, false, dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);

   sv = dtl_sv_new();
   CuAssertPtrNotNull(tc, sv);
   dtl_sv_set_bool(sv, true);
   CuAssertIntEquals(tc, DTL_SV_BOOL, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, true, dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);

   sv = dtl_sv_make_bool(true);
   CuAssertIntEquals(tc, DTL_SV_BOOL, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, true, dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);

   sv = dtl_sv_make_bool(false);
   CuAssertIntEquals(tc, DTL_SV_BOOL, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, false, dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);
}

static void test_dtl_sv_lt_i32(CuTest *tc)
{
   dtl_sv_t *a = dtl_sv_make_i32(-140);
   dtl_sv_t *b = dtl_sv_make_i32(0);
   bool ltres = false;

   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(a, b, &ltres));
   CuAssertTrue(tc, ltres);

   dtl_dec_ref(a);
   dtl_dec_ref(b);
}

static void test_dtl_sv_lt_str(CuTest *tc)
{
   dtl_sv_t *a = dtl_sv_make_cstr("Hello");
   dtl_sv_t *b = dtl_sv_make_cstr("World");
   bool ltres = false;

   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(a, b, &ltres));
   CuAssertTrue(tc, ltres);

   dtl_dec_ref(a);
   dtl_dec_ref(b);
}

static void test_dtl_sv_make_set_str(CuTest *tc)
{
   bool ok = false;
   dtl_sv_t *sv = dtl_sv_make_cstr("testing");
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, DTL_SV_STR, (int) dtl_sv_type(sv));
   CuAssertStrEquals(tc, "testing", dtl_sv_to_cstr(sv, &ok));
   CuAssertTrue(tc, ok);

   dtl_sv_set_cstr(sv, "updated");
   CuAssertStrEquals(tc, "updated", dtl_sv_to_cstr(sv, &ok));

   adt_str_t *adt_s = adt_str_new_cstr("adt_str_val");
   dtl_sv_set_str(sv, adt_s);
   CuAssertStrEquals(tc, "adt_str_val", dtl_sv_to_cstr(sv, &ok));

   dtl_sv_t *sv_clone = dtl_sv_make_str(adt_s);
   CuAssertStrEquals(tc, "adt_str_val", dtl_sv_to_cstr(sv_clone, &ok));
   dtl_dec_ref(sv_clone);
   adt_str_delete(adt_s);

   const uint8_t slice[] = "abcdef";
   dtl_sv_set_bstr(sv, &slice[1], &slice[4]);
   CuAssertStrEquals(tc, "bcd", dtl_sv_to_cstr(sv, &ok));

   dtl_dec_ref(sv);
}

static void test_dtl_sv_numeric_setters(CuTest *tc)
{
   dtl_sv_t *sv = dtl_sv_new();
   CuAssertPtrNotNull(tc, sv);

   dtl_sv_set_i32(sv, -50);
   CuAssertIntEquals(tc, DTL_SV_I32, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, -50, dtl_sv_to_i32(sv, NULL));

   dtl_sv_set_u32(sv, 500u);
   CuAssertIntEquals(tc, DTL_SV_U32, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, 500, (int) dtl_sv_to_u32(sv, NULL));

   dtl_sv_set_i64(sv, -10000000000LL);
   CuAssertIntEquals(tc, DTL_SV_I64, (int) dtl_sv_type(sv));
   CuAssertTrue(tc, -10000000000LL == dtl_sv_to_i64(sv, NULL));

   dtl_sv_set_u64(sv, 10000000000ULL);
   CuAssertIntEquals(tc, DTL_SV_U64, (int) dtl_sv_type(sv));
   CuAssertTrue(tc, 10000000000ULL == dtl_sv_to_u64(sv, NULL));

   dtl_sv_set_flt(sv, 3.14f);
   CuAssertIntEquals(tc, DTL_SV_FLT, (int) dtl_sv_type(sv));
   CuAssertDblEquals(tc, 3.14, (double) dtl_sv_to_flt(sv, NULL), 0.01);

   dtl_sv_set_dbl(sv, 2.71828);
   CuAssertIntEquals(tc, DTL_SV_DBL, (int) dtl_sv_type(sv));
   CuAssertDblEquals(tc, 2.71828, dtl_sv_to_dbl(sv, NULL), 0.0001);

   dtl_sv_set_char(sv, 'Z');
   CuAssertIntEquals(tc, DTL_SV_CHAR, (int) dtl_sv_type(sv));
   CuAssertIntEquals(tc, 'Z', dtl_sv_to_char(sv, NULL));

   dtl_dec_ref(sv);
}

static int g_destructor_called = 0;
static void dummy_destructor(void *arg)
{
   (void) arg;
   g_destructor_called++;
}

static void test_dtl_sv_ptr_destructor(CuTest *tc)
{
   g_destructor_called = 0;
   int dummy_val = 123;
   dtl_sv_t *sv = dtl_sv_make_ptr(&dummy_val, dummy_destructor);
   CuAssertPtrNotNull(tc, sv);
   CuAssertIntEquals(tc, 0, g_destructor_called);

   // Overwrite pointer with setter
   int dummy_val2 = 456;
   dtl_sv_set_ptr(sv, &dummy_val2, dummy_destructor);
   dtl_dec_ref(sv);
   CuAssertTrue(tc, g_destructor_called >= 1);
}

static void test_dtl_sv_dv_wrapper(CuTest *tc)
{
   dtl_av_t *av = dtl_av_new();
   dtl_sv_t *sv = dtl_sv_make_dv((dtl_dv_t *) av, false);
   CuAssertIntEquals(tc, DTL_DV_ARRAY, (int) dtl_sv_dv_type(sv));
   CuAssertPtrEquals(tc, av, dtl_sv_to_av(sv));
   CuAssertPtrEquals(tc, NULL, dtl_sv_to_hv(sv));
   CuAssertPtrEquals(tc, (dtl_dv_t *) av, dtl_sv_to_dv(sv));
   dtl_dec_ref(sv);

   dtl_hv_t *hv = dtl_hv_new();
   sv = dtl_sv_make_dv((dtl_dv_t *) hv, false);
   CuAssertIntEquals(tc, DTL_DV_HASH, (int) dtl_sv_dv_type(sv));
   CuAssertPtrEquals(tc, hv, dtl_sv_to_hv(sv));
   dtl_dec_ref(sv);
}

static void test_dtl_sv_conversions(CuTest *tc)
{
   bool ok = false;

   // Number to string
   dtl_sv_t *sv = dtl_sv_make_i32(42);
   CuAssertStrEquals(tc, "42", dtl_sv_to_cstr(sv, &ok));
   CuAssertTrue(tc, ok);

   adt_str_t *str = dtl_sv_to_str(sv, &ok);
   CuAssertPtrNotNull(tc, str);
   CuAssertStrEquals(tc, "42", adt_str_cstr(str));
   adt_str_delete(str);
   dtl_dec_ref(sv);

   // String "true" to bool
   sv = dtl_sv_make_cstr("true");
   CuAssertTrue(tc, dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);

   // String "false" to bool
   sv = dtl_sv_make_cstr("false");
   CuAssertTrue(tc, !dtl_sv_to_bool(sv, &ok));
   CuAssertTrue(tc, ok);
   dtl_dec_ref(sv);

   // Invalid string to bool
   sv = dtl_sv_make_cstr("not_a_bool");
   ok = true;
   dtl_sv_to_bool(sv, &ok);
   CuAssertTrue(tc, !ok);
   dtl_dec_ref(sv);

   // None to number
   sv = dtl_sv_new();
   ok = true;
   dtl_sv_to_i32(sv, &ok);
   CuAssertTrue(tc, !ok);
   dtl_dec_ref(sv);
}

static void test_dtl_sv_lt_comparisons(CuTest *tc)
{
   bool res = false;

   // u32
   dtl_sv_t *u1 = dtl_sv_make_u32(10);
   dtl_sv_t *u2 = dtl_sv_make_u32(20);
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(u1, u2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(u1);
   dtl_dec_ref(u2);

   // i64
   dtl_sv_t *i1 = dtl_sv_make_i64(-100LL);
   dtl_sv_t *i2 = dtl_sv_make_i64(100LL);
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(i1, i2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(i1);
   dtl_dec_ref(i2);

   // u64
   dtl_sv_t *u64_1 = dtl_sv_make_u64(1000ULL);
   dtl_sv_t *u64_2 = dtl_sv_make_u64(2000ULL);
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(u64_1, u64_2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(u64_1);
   dtl_dec_ref(u64_2);

   // flt
   dtl_sv_t *f1 = dtl_sv_make_flt(1.5f);
   dtl_sv_t *f2 = dtl_sv_make_flt(2.5f);
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(f1, f2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(f1);
   dtl_dec_ref(f2);

   // dbl
   dtl_sv_t *d1 = dtl_sv_make_dbl(1.5);
   dtl_sv_t *d2 = dtl_sv_make_dbl(2.5);
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(d1, d2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(d1);
   dtl_dec_ref(d2);

   // char
   dtl_sv_t *c1 = dtl_sv_make_char('A');
   dtl_sv_t *c2 = dtl_sv_make_char('B');
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_sv_lt(c1, c2, &res));
   CuAssertTrue(tc, res);
   dtl_dec_ref(c1);
   dtl_dec_ref(c2);

   // type mismatch
   dtl_sv_t *s = dtl_sv_make_cstr("test");
   dtl_sv_t *num = dtl_sv_make_i32(10);
   CuAssertIntEquals(tc, DTL_TYPE_ERROR, dtl_sv_lt(s, num, &res));
   dtl_dec_ref(s);
   dtl_dec_ref(num);
}

static void test_dtl_sv_take_bytes(CuTest *tc)
{
   const uint8_t data[] = {1, 2, 3, 4};
   adt_bytes_t *bytes = adt_bytes_new(data, sizeof(data));
   dtl_sv_t *sv = dtl_sv_new();
   dtl_sv_take_bytes(sv, bytes);
   CuAssertIntEquals(tc, DTL_SV_BYTES, (int) dtl_sv_type(sv));
   const adt_bytes_t *read_bytes = dtl_sv_get_bytes(sv);
   CuAssertPtrEquals(tc, bytes, (void *) read_bytes);
   dtl_dec_ref(sv); // frees bytes
}
