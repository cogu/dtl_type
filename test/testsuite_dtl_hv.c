/*****************************************************************************
* \file      testsuite_dtl_hv.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit tests for DTL hash
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
static void test_dtl_hv_new_delete(CuTest* tc);
static void test_dtl_hv_get_cstr_set(CuTest* tc);
static void test_dtl_hv_keys_sorted(CuTest* tc);

//////////////////////////////////////////////////////////////////////////////
// PRIVATE VARIABLES
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite* testsuite_dtl_hv(void)
{
   CuSuite* suite = CuSuiteNew();

   SUITE_ADD_TEST(suite, test_dtl_hv_new_delete);
   SUITE_ADD_TEST(suite, test_dtl_hv_get_cstr_set);
   SUITE_ADD_TEST(suite, test_dtl_hv_keys_sorted);

   return suite;
}
//////////////////////////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
//////////////////////////////////////////////////////////////////////////////

static void test_dtl_hv_new_delete(CuTest* tc)
{
	dtl_hv_t *hv = dtl_hv_new();
	CuAssertPtrNotNull(tc, hv);
	dtl_hv_delete(hv);
}

void test_dtl_hv_get_cstr_set(CuTest* tc)
{
	dtl_hv_t *hv = dtl_hv_new();
	CuAssertPtrNotNull(tc, hv);
	dtl_sv_t *sv = dtl_sv_make_i32(82);
	dtl_hv_set_cstr(hv,"First",(dtl_dv_t*) dtl_sv_make_i32(1), false);
	dtl_hv_set_cstr(hv,"Second",(dtl_dv_t*) dtl_sv_make_i32(2), false);
	dtl_hv_set_cstr(hv,"Third",(dtl_dv_t*) dtl_sv_make_i32(4), false);
	dtl_hv_set_cstr(hv,"Fourth",(dtl_dv_t*) sv, false);
	dtl_inc_ref(sv);
	CuAssertIntEquals(tc,4,dtl_hv_length(hv));
	CuAssertIntEquals(tc,2,dtl_ref_cnt(sv));

	dtl_dv_t *first = dtl_hv_get_cstr(hv,"First");
	dtl_dv_t *second = dtl_hv_get_cstr(hv,"Second");
	dtl_dv_t *third = dtl_hv_get_cstr(hv,"Third");
	dtl_dv_t *fourth = dtl_hv_get_cstr(hv,"Fourth");
	CuAssertPtrNotNull(tc,first);
	CuAssertPtrNotNull(tc,second);
	CuAssertPtrNotNull(tc,third);
	CuAssertPtrEquals(tc,sv,fourth);
	dtl_dec_ref(hv);
	CuAssertIntEquals(tc, 82, dtl_sv_to_i32((dtl_sv_t*)fourth, NULL));
	CuAssertIntEquals(tc,1,dtl_ref_cnt(sv));
	dtl_dec_ref(sv);
}

static void test_dtl_hv_keys_sorted(CuTest* tc)
{
   dtl_hv_t *hv = dtl_hv_new();
   dtl_av_t *keys = 0;
   CuAssertPtrNotNull(tc, hv);

   dtl_hv_set_cstr(hv,"Illinois",(dtl_dv_t*) dtl_sv_make_dbl(3.85), false);
   dtl_hv_set_cstr(hv,"Pennsylvania",(dtl_dv_t*) dtl_sv_make_dbl(3.87), false);
   dtl_hv_set_cstr(hv,"Florida",(dtl_dv_t*) dtl_sv_make_dbl(6.44), false);
   dtl_hv_set_cstr(hv,"Ohio",(dtl_dv_t*) dtl_sv_make_dbl(3.53), false);
   dtl_hv_set_cstr(hv,"California",(dtl_dv_t*) dtl_sv_make_dbl(11.96), false);
   dtl_hv_set_cstr(hv,"Texas",(dtl_dv_t*) dtl_sv_make_dbl(8.68), false);
   CuAssertIntEquals(tc, 6, dtl_hv_length(hv));

   keys = dtl_hv_keys(hv);
   CuAssertPtrNotNull(tc, keys);
   CuAssertIntEquals(tc, 6, dtl_av_length(keys));
   CuAssertIntEquals(tc, DTL_NO_ERROR, dtl_av_sort(keys, NULL, false));
   CuAssertStrEquals(tc, "California", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 0)));
   CuAssertStrEquals(tc, "Florida", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 1)));
   CuAssertStrEquals(tc, "Illinois", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 2)));
   CuAssertStrEquals(tc, "Ohio", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 3)));
   CuAssertStrEquals(tc, "Pennsylvania", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 4)));
   CuAssertStrEquals(tc, "Texas", dtl_sv_to_cstr((dtl_sv_t*) dtl_av_value(keys, 5)));

   dtl_dec_ref(keys);
   dtl_dec_ref(hv);
}
