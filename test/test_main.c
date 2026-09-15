/*****************************************************************************
* \file      test_main.c
* \author    Conny Gustafsson
* \date      2013-08-16
* \brief     Unit test entry point for dtl_type
*
* Copyright (c) 2013-2026 Conny Gustafsson
* SPDX-License-Identifier: MIT
* See LICENSE in project root for full license terms.
******************************************************************************/

//////////////////////////////////////////////////////////////////////////////
// INCLUDES
//////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include "CuTest.h"
#ifdef MEM_LEAK_CHECK
#include "CMemLeak.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// FORWARD DECLARATIONS
//////////////////////////////////////////////////////////////////////////////
CuSuite *testsuite_dtl_dv(void);
CuSuite *testsuite_dtl_sv(void);
CuSuite *testsuite_dtl_av(void);
CuSuite *testsuite_dtl_hv(void);

//////////////////////////////////////////////////////////////////////////////
// MAIN RUNNER
//////////////////////////////////////////////////////////////////////////////
static void run_all_tests(void)
{
   CuString *output = CuStringNew();
   CuSuite *suite = CuSuiteNew();

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
   run_all_tests();
   return 0;
}
