/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"
#include "../../src/ctest_core.h"

TEST(TestResultTests, createTestResult, Tests that a CTest_TestResult is created successfully)
{
	const char *logs = "logs\0";
	CTest_TestResult *testResult = createTestResult(CTEST_SUCCESS, "logs\0", 5);
	CT_ASSERT_NON_NULL(testResult);
	CT_ASSERT_TRUE(CTEST_SUCCESS == testResult->status);
	CT_ASSERT_NON_NULL(testResult->logs);
	CT_ASSERT_STRING_EQUALS(logs, testResult->logs);
	free(testResult->logs);
	testResult->logs = NULL;
	free(testResult);
	testResult = NULL;
}

TEST(TestResultTests, createTestResultWithoutLogs, Tests that a CTest_TestResult is created successfully when no logs are given)
{
	CTest_TestResult *testResult = createTestResult(CTEST_SUCCESS, NULL, 0);
	CT_ASSERT_NON_NULL(testResult);
	CT_ASSERT_TRUE(CTEST_SUCCESS == testResult->status);
	CT_ASSERT_NULL(testResult->logs);
	free(testResult);
	testResult = NULL;
}

TEST(TestResultTests, freeTestResultWithLogs, Tests that a CTest_TestResult is freed correctly when it has logs)
{
	const char *logs = "logs\0";
	CTest_TestResult *testResult = createTestResult(CTEST_CRASH_SEGV, logs, 5);
	CT_ASSERT_NON_NULL(testResult);
	CT_ASSERT_NON_NULL(testResult->logs);
	CT_ASSERT_STRING_EQUALS(logs, testResult->logs);
	freeTestResult(&testResult);
	CT_ASSERT_NULL(testResult);	
}

TEST(TestResultTests, freeTestResultWithoutLogs, Tests that a CTest_TestResult is freed correctly when it does not have logs)
{
	CTest_TestResult *testResult = createTestResult(CTEST_CRASH_SEGV, NULL, 0);
	CT_ASSERT_NON_NULL(testResult);
	CT_ASSERT_NULL(testResult->logs);
	freeTestResult(&testResult);
	CT_ASSERT_NULL(testResult);	
}
