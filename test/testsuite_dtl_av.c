/*****************************************************************************
* \file      testsuite_dtl_av.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for DTL array
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
// PRIVATE CONSTANTS AND DATA TYPES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTION PROTOTYPES
//////////////////////////////////////////////////////////////////////////////
static void test_dtl_av_new_delete(CuTest* tc);
static void test_dtl_av_push_pop(CuTest* tc);
static void test_dtl_av_get_set(CuTest* tc);
static void test_dtl_av_sort_i32(CuTest* tc);
static void test_dtl_av_sort_strings(CuTest* tc);

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite* testsuite_dtl_av(void)
{
   CuSuite* suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_av_new_delete);
   SUITE_ADD_TEST(suite, test_dtl_av_push_pop);
   SUITE_ADD_TEST(suite, test_dtl_av_get_set);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_i32);
   SUITE_ADD_TEST(suite, test_dtl_av_sort_strings);

   return suite;
}
//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_av_new_delete(CuTest* tc)
{
	dtl_av_t *av = dtl_av_new();
	CuAssertPtrNotNull(tc, av);
	dtl_av_delete(av);
}

static void test_dtl_av_push_pop(CuTest* tc)
{
	dtl_av_t *av = dtl_av_new();
	CuAssertPtrNotNull(tc, av);
	dtl_sv_t *sv = dtl_sv_make_i32(82);
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(1), false);
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(2), false);
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(4), false);
	dtl_av_push(av,(dtl_dv_t*) sv, false);
	dtl_inc_ref(sv);
	CuAssertIntEquals(tc,2,dtl_ref_cnt(sv));
	dtl_dec_ref(av);
	CuAssertIntEquals(tc,1,dtl_ref_cnt(sv));
	dtl_dec_ref(sv);
}

static void test_dtl_av_get_set(CuTest* tc)
{
	dtl_av_t *av = dtl_av_new();

	CuAssertPtrNotNull(tc, av);

	dtl_sv_t *sv = dtl_sv_make_i32(1);
	dtl_av_set(av, 3, (dtl_dv_t*) sv);
	CuAssertPtrEquals(tc,&g_dtl_sv_none,*dtl_av_get(av,0));
	CuAssertPtrEquals(tc,&g_dtl_sv_none,*dtl_av_get(av,1));
	CuAssertPtrEquals(tc,&g_dtl_sv_none,*dtl_av_get(av,2));
	dtl_dv_t *dv = *dtl_av_get(av, 3);
	CuAssertPtrEquals(tc, dv, sv);
	CuAssertIntEquals(tc, DTL_DV_SCALAR, dtl_dv_type(dv));
	CuAssertIntEquals(tc, 1, dtl_sv_to_i32( (dtl_sv_t*) dv, NULL));

	dtl_dec_ref(av);
}


static void test_dtl_av_sort_i32(CuTest* tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);

   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(9), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(2), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(5), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(10), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(4), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_i32(7), false);
   CuAssertIntEquals(tc, 6, dtl_av_length(av));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(av, NULL, false));
   CuAssertIntEquals(tc, 2, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 0), NULL));
   CuAssertIntEquals(tc, 4, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 1), NULL));
   CuAssertIntEquals(tc, 5, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 2), NULL));
   CuAssertIntEquals(tc, 7, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 3), NULL));
   CuAssertIntEquals(tc, 9, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 4), NULL));
   CuAssertIntEquals(tc, 10, dtl_sv_to_i32((dtl_sv_t*) dtl_av_value(av, 5), NULL));

   dtl_dec_ref(av);
}

static void test_dtl_av_sort_strings(CuTest* tc)
{
   dtl_av_t *av = dtl_av_new();
   CuAssertPtrNotNull(tc, av);

   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_cstr("strawberry"), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_cstr("apple"), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_cstr("raspberry"), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_cstr("pear"), false);
   dtl_av_push(av, (dtl_dv_t*) dtl_sv_make_cstr("pineapple"), false);
   CuAssertIntEquals(tc, 5, dtl_av_length(av));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(av, NULL, false));
   CuAssertStrEquals(tc, "apple", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(av, 0)));
   CuAssertStrEquals(tc, "pear", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(av, 1)));
   CuAssertStrEquals(tc, "pineapple", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(av, 2)));
   CuAssertStrEquals(tc, "raspberry", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(av, 3)));
   CuAssertStrEquals(tc, "strawberry", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(av, 4)));

   dtl_dec_ref(av);
}


