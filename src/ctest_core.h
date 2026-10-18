/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#ifndef __CTEST_CORE_H_
#define __CTEST_CORE_H_

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * This header file defines the api for automated testing of the emulator
 */


#define CTEST_VERSION "0.1.0"
#define CTEST_COPYRIGHT "Copyright (c) 2026 Miguel Blanco Godón"

#define CTEST_COLOR_RED "\x1b[31m"
#define CTEST_COLOR_GREEN "\x1b[32m"
#define CTEST_COLOR_RESET "\x1b[0m"

#define CTEST_SUCCESS_MESSAGE "[SUCCESS]"
#define CTEST_FAILURE_MESSAGE "[FAILURE]"
#define CTEST_TEST_RESULT_MESSAGE_SIZE 9
/**
 * Function pointer to the executable test function for each test
 */
typedef void (*CTest_TestFunction)(void);

/**
 * Test execution result status
 */
typedef enum
{
	CTEST_SUCCESS,
	CTEST_FAILURE,
	CTEST_CRASH_SEGV,
	CTEST_CRASH_OTHER
} CTest_TestExecutionStatus;

/**
 * Test execution info, contains test execution status and logs (stderr, stdout)
 * written during the test execution
 */
typedef struct CTest_TestResult
{
	CTest_TestExecutionStatus status;
	char *logs;
} CTest_TestResult;

/**
 * Stores information and status of each test
 */
typedef struct CTest_TestCase
{
	const char *name;
	const char *suiteName;
	const char *description;
	const char *expectedError;
	bool errorIsRegexp;
	bool executed;
	// function pointer to the test function the test executes
	CTest_TestFunction execute;
	CTest_TestResult *testResult;
} CTest_TestCase;

/**
 * Stores information and status of a test suite
 * It also stores pointers to each individual test case.
 */
typedef struct CTest_TestSuite
{
	const char *name;
	CTest_TestCase **testCases;
	long testCaseSize;
	long testCount;
	long successfulTests;
	long failedTests;
} CTest_TestSuite;

/**
 * Stores information about the status of the tests
 * Contains pointers to all suites
 */
typedef struct CTest_TestStatus
{
	CTest_TestSuite **testSuites;
	long testSuitesSize;
	long suiteCount;
	long successfulTests;
	long failedTests;
} CTest_TestStatus;

// global test state
extern CTest_TestStatus *testStatus;

// Non documented functions

/**
 * Creates a CTest_TestResult element and initializes its content
 */
CTest_TestResult *createTestResult(CTest_TestExecutionStatus status, const char *logs, size_t logSize);

/**
 * Frees all the allocated memory in a CTest_TestResult element
 */
void freeTestResult(CTest_TestResult **testResult);
/**
 * Creates a CTest_TestCase element and initilizes its content
 */
CTest_TestCase * createTestCase(const char *testName, const char *testSuite, 
		const char *description, CTest_TestFunction testFunction);
/**
 * Frees all the allocated memory in a CTest_TestCase element
 */
void freeTestCase(CTest_TestCase *testCase);

/**
 * Creates a CTest_TestSuite elements and initializes its content
 */
CTest_TestSuite * createTestSuite(const char *name);
/**
 * Free all the allocated memory in a CTest_TestSuite element.
 * Frees all the CTest_TestCase elements inside the CTest_TestSuite.
 */ 
void freeTestSuite(CTest_TestSuite **testSuitePtr);

/**
 * Adds a CTest_TestCase to a CTest_TestSuite 
 */
bool addTestCase(CTest_TestSuite *testSuite, CTest_TestCase *testCase);
/**
 * Adds a CTest_TestSuite to the CTest_TestStatus object
 */
bool addTestSuite(CTest_TestStatus *testStatus, CTest_TestSuite *testSuite);

static void __attribute__((unused)) registerTestInSuite(CTest_TestSuite *testSuite, const char *testName,
		const char *description, const char *expectedError, bool errorIsRegexp, CTest_TestFunction testFunction)
{
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot register test: testSuite is NULL\n");
		return;
	}
	CTest_TestCase *testCase = createTestCase(testName, testSuite->name, description, testFunction);
	testCase->expectedError = expectedError;
	testCase->errorIsRegexp = errorIsRegexp;
	if (!addTestCase(testSuite, testCase)) {
		freeTestCase(testCase);
		testCase = NULL;
	}
}

static inline CTest_TestSuite * __attribute__((unused)) findSuite(CTest_TestStatus *testStatus, const char *suiteName)
{
	if (testStatus == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot find suite '%s': testStatus is NULL\n", suiteName);
		return NULL;
	}
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		if (strcmp(testStatus->testSuites[i]->name, suiteName) == 0) {
			return testStatus->testSuites[i];
		}
	}
	return NULL;
}

#endif // __CTEST_CORE_H
