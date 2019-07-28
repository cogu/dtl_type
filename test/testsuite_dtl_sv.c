#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "CuTest.h"
#include "dtl_sv.h"
#include "CMemLeak.h"


void test_dtl_sv_create(CuTest* tc)
{
	dtl_sv_t *sv = dtl_sv_new();
	CuAssertPtrNotNull(tc, sv);
	dtl_sv_delete(sv);
}

void test_dtl_sv_make(CuTest* tc)
{
	dtl_sv_t *sv;

	//int32_t
	sv = dtl_sv_make_i32(124);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_I32, dtl_sv_type(sv));
	CuAssertIntEquals(tc, 124, dtl_sv_to_i32(sv, NULL));
	dtl_sv_delete(sv);

	//uint32_t
	sv = dtl_sv_make_u32(8328);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_U32, dtl_sv_type(sv));
	CuAssertIntEquals(tc, 8328, dtl_sv_to_u32(sv, NULL));
	dtl_sv_delete(sv);

	//int64_t
	sv = dtl_sv_make_i64(-1375713549903L);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_I64, dtl_sv_type(sv));
	CuAssertTrue(tc, -1375713549903LL == dtl_sv_to_i64(sv, NULL) );
	dtl_sv_delete(sv);

	//uint64_t
	sv = dtl_sv_make_u64(1375713549903UL);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_U64, dtl_sv_type(sv));
	CuAssertTrue(tc, 1375713549903ULL == dtl_sv_to_u64(sv, NULL));
	dtl_sv_delete(sv);

	//flt
	sv = dtl_sv_make_flt(64.0);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_FLT, dtl_sv_type(sv));
	CuAssertDblEquals(tc, 64.0, (double) dtl_sv_to_flt(sv, NULL), 0.001);
	dtl_sv_delete(sv);

	//dbl
	sv = dtl_sv_make_dbl(83.0);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_DBL, dtl_sv_type(sv));
	CuAssertDblEquals(tc, 83.0, dtl_sv_to_dbl(sv, NULL), 0.001);
	dtl_sv_delete(sv);

	//ptr
	int i = 825;
	sv = dtl_sv_make_ptr(&i, NULL);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_PTR, dtl_sv_type(sv));
	CuAssertPtrEquals(tc, &i, dtl_sv_to_ptr(sv));
	dtl_sv_delete(sv);

	//dv
	sv = dtl_sv_make_i32(0);
	CuAssertPtrNotNull(tc, sv);
	CuAssertIntEquals(tc, DTL_SV_I32, dtl_sv_type(sv));
	dtl_sv_t *sv2 = dtl_sv_make_dv((dtl_dv_t*) sv, true);
	CuAssertPtrNotNull(tc, sv2);
	CuAssertIntEquals(tc, DTL_SV_DV, dtl_sv_type(sv2));
	CuAssertPtrEquals(tc, sv, dtl_sv_to_sv(sv2));
	CuAssertIntEquals(tc, 1, dtl_ref_cnt(sv2));
	CuAssertIntEquals(tc, 2, dtl_ref_cnt(sv));
	dtl_dec_ref(sv2);
	CuAssertIntEquals(tc, 1, dtl_ref_cnt(sv));
	dtl_dec_ref(sv);
}

void test_dtl_sv_bool(CuTest* tc){
	dtl_sv_t *sv;

	sv = dtl_sv_new();
	CuAssertPtrNotNull(tc, sv);
	dtl_sv_set_bool(sv, false);
	CuAssertIntEquals(tc, DTL_SV_BOOL, dtl_sv_type(sv));
	CuAssertIntEquals(tc, false, dtl_sv_to_bool(sv));
	dtl_dec_ref(sv);

	sv = dtl_sv_new();
	CuAssertPtrNotNull(tc, sv);
	dtl_sv_set_bool(sv, true);
	CuAssertIntEquals(tc, DTL_SV_BOOL, dtl_sv_type(sv));
	CuAssertIntEquals(tc, true, dtl_sv_to_bool(sv));
	dtl_dec_ref(sv);

	sv = dtl_sv_make_bool(true);
	CuAssertIntEquals(tc, DTL_SV_BOOL, dtl_sv_type(sv));
	CuAssertIntEquals(tc, true, dtl_sv_to_bool(sv));
	dtl_dec_ref(sv);


	sv = dtl_sv_make_bool(false);
	CuAssertIntEquals(tc, DTL_SV_BOOL, dtl_sv_type(sv));
	CuAssertIntEquals(tc, false, dtl_sv_to_bool(sv));
	dtl_dec_ref(sv);

}


CuSuite* testsuite_dtl_sv(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_dtl_sv_create);
	SUITE_ADD_TEST(suite, test_dtl_sv_make);
	SUITE_ADD_TEST(suite, test_dtl_sv_bool);
	return suite;
}

