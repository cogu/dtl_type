#include <assert.h>
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "CuTest.h"
#include "dtl_sv.h"
#include "CMemLeak.h"


void test_dtl_dv_null(CuTest* tc){
	dtl_dv_t *dv = dtl_dv_null();
	CuAssertPtrNotNull(tc,dv);
	CuAssertIntEquals(tc,DTL_DV_NULL,dv->u32Flags);
	CuAssertIntEquals(tc,1,dv->u32RefCnt);
	dtl_ref_dec(dv);
}



CuSuite* testsuite_dtl_dv(void)
{
	CuSuite* suite = CuSuiteNew();

	SUITE_ADD_TEST(suite, test_dtl_dv_null);
	return suite;
}

