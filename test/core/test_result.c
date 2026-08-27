/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"
#include "../../src/ctest_core.h"

TEST(TestResultTests, createTestResult, Tests that a TestResult is created successfully)
{
	const char *logs = "logs\0";
	TestResult *testResult = createTestResult(SUCCESS, "logs\0", 5);
	ASSERT_NON_NULL(testResult);
	ASSERT_TRUE(SUCCESS == testResult->status);
	ASSERT_NON_NULL(testResult->logs);
	ASSERT_STRING_EQUALS(logs, testResult->logs);
	free(testResult->logs);
	testResult->logs = NULL;
	free(testResult);
	testResult = NULL;
}

TEST(TestResultTests, createTestResultWithoutLogs, Tests that a TestResult is created successfully when no logs are given)
{
	TestResult *testResult = createTestResult(SUCCESS, NULL, 0);
	ASSERT_NON_NULL(testResult);
	ASSERT_TRUE(SUCCESS == testResult->status);
	ASSERT_NULL(testResult->logs);
	free(testResult);
	testResult = NULL;
}

TEST(TestResultTests, freeTestResultWithLogs, Tests that a TestResult is freed correctly when it has logs)
{
	const char *logs = "logs\0";
	TestResult *testResult = createTestResult(CRASH_SEGV, logs, 5);
	ASSERT_NON_NULL(testResult);
	ASSERT_NON_NULL(testResult->logs);
	ASSERT_STRING_EQUALS(logs, testResult->logs);
	freeTestResult(&testResult);
	ASSERT_NULL(testResult);	
}

TEST(TestResultTests, freeTestResultWithoutLogs, Tests that a TestResult is freed correctly when it does not have logs)
{
	TestResult *testResult = createTestResult(CRASH_SEGV, NULL, 0);
	ASSERT_NON_NULL(testResult);
	ASSERT_NULL(testResult->logs);
	freeTestResult(&testResult);
	ASSERT_NULL(testResult);	
}
