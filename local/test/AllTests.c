#include <stdio.h>
#include <malloc.h>
#include "CuTest.h"
#include "CMemLeak.h"


CuSuite* testsuite_dtl_dv(void);
CuSuite* testsuite_dtl_sv(void);
CuSuite* testsuite_dtl_av(void);
CuSuite* testsuite_dtl_hv(void);

void vfree(void *arg)
{
   free(arg);
}


void RunAllTests(void)
{
	CuString *output = CuStringNew();
	CuSuite* suite = CuSuiteNew();

	CuSuiteAddSuite(suite, testsuite_dtl_dv());
	CuSuiteAddSuite(suite, testsuite_dtl_sv());
	CuSuiteAddSuite(suite, testsuite_dtl_av());
	CuSuiteAddSuite(suite, testsuite_dtl_hv());

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("%s\n", output->buffer);
	CuSuiteDelete(suite);
	CuStringDelete(output);
}

int main(void)
{
	RunAllTests();
	return 0;
}

