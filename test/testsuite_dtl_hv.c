#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "CuTest.h"
#include "dtl_sv.h"
#include "dtl_hv.h"
#include "CMemLeak.h"


void test_dtl_hv_new_delete(CuTest* tc)
{
	dtl_hv_t *hv = dtl_hv_new();
	CuAssertPtrNotNull(tc, hv);
	dtl_hv_delete(hv);
}

void test_dtl_hv_get_set(CuTest* tc){
	dtl_hv_t *hv = dtl_hv_new();
	CuAssertPtrNotNull(tc, hv);
	dtl_sv_t *sv = dtl_sv_make_i32(82);
	dtl_hv_set(hv,"First",(dtl_dv_t*) dtl_sv_make_i32(1));
	dtl_hv_set(hv,"Second",(dtl_dv_t*) dtl_sv_make_i32(2));
	dtl_hv_set(hv,"Third",(dtl_dv_t*) dtl_sv_make_i32(4));
	dtl_hv_set(hv,"Fourth",(dtl_dv_t*) sv);
	dtl_ref_inc(sv);
	CuAssertIntEquals(tc,4,dtl_hv_length(hv));
	CuAssertIntEquals(tc,2,dtl_ref_cnt(sv));

	dtl_dv_t *first = dtl_hv_get(hv,"First");
	dtl_dv_t *second = dtl_hv_get(hv,"Second");
	dtl_dv_t *third = dtl_hv_get(hv,"Third");
	dtl_dv_t *fourth = dtl_hv_get(hv,"Fourth");
	CuAssertPtrNotNull(tc,first);
	CuAssertPtrNotNull(tc,second);
	CuAssertPtrNotNull(tc,third);
	CuAssertPtrEquals(tc,sv,fourth);
	dtl_ref_dec(hv);
	CuAssertIntEquals(tc, 82, dtl_sv_to_i32((dtl_sv_t*)fourth, NULL));
	CuAssertIntEquals(tc,1,dtl_ref_cnt(sv));
	dtl_ref_dec(sv);
}





CuSuite* testsuite_dtl_hv(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_dtl_hv_new_delete);
	SUITE_ADD_TEST(suite, test_dtl_hv_get_set);

	return suite;
}

