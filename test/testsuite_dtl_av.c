#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "CuTest.h"
#include "dtl_sv.h"
#include "dtl_av.h"
#include "CMemLeak.h"


void test_dtl_av_new_delete(CuTest* tc)
{
	dtl_av_t *av = dtl_av_new();
	CuAssertPtrNotNull(tc, av);
	dtl_av_delete(av);
}

void test_dtl_av_push_pop(CuTest* tc){
	dtl_av_t *av = dtl_av_new();
	CuAssertPtrNotNull(tc, av);
	dtl_sv_t *sv = dtl_sv_make_i32(82);
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(1));
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(2));
	dtl_av_push(av,(dtl_dv_t*) dtl_sv_make_i32(4));
	dtl_av_push(av,(dtl_dv_t*) sv);
	dtl_ref_inc(sv);
	CuAssertIntEquals(tc,2,dtl_ref_cnt(sv));
	dtl_ref_dec(av);
	CuAssertIntEquals(tc,1,dtl_ref_cnt(sv));
	dtl_ref_dec(sv);
}

void test_dtl_av_get_set(CuTest* tc){
	dtl_av_t *av = dtl_av_new();

	CuAssertPtrNotNull(tc, av);

	dtl_sv_t *sv = dtl_sv_make_i32(1);
	dtl_av_set(av, 3, (dtl_dv_t*) sv);
	CuAssertPtrEquals(tc,&g_dtl_sv_undef,*dtl_av_get(av,0));
	CuAssertPtrEquals(tc,&g_dtl_sv_undef,*dtl_av_get(av,1));
	CuAssertPtrEquals(tc,&g_dtl_sv_undef,*dtl_av_get(av,2));
	dtl_dv_t *dv = *dtl_av_get(av, 3);
	CuAssertPtrEquals(tc, dv, sv);
	CuAssertIntEquals(tc, DTL_DV_SCALAR, dtl_dv_type(dv));
	CuAssertIntEquals(tc, 1, dtl_sv_to_i32( (dtl_sv_t*) dv, NULL));

	dtl_ref_dec(av);
}




CuSuite* testsuite_dtl_av(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_dtl_av_new_delete);
	SUITE_ADD_TEST(suite, test_dtl_av_push_pop);
	SUITE_ADD_TEST(suite, test_dtl_av_get_set);
	return suite;
}

