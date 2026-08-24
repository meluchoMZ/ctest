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

#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"

#define SUCCESS_MESSAGE "[SUCCESS]"
#define FAILURE_MESSAGE "[FAILURE]"
#define TEST_RESULT_MESSAGE_SIZE 9
/**
 * Function pointer to the executable test function for each test
 */
typedef void (*TestFunction)(void);

/**
 * Test execution result status
 */
typedef enum
{
	SUCCESS,
	FAILURE,
	CRASH_SEGV,
	CRASH_OTHER
} TestResult;

/**
 * Stores information and status of each test
 */
typedef struct TestCase
{
	const char *name;
	const char *suiteName;
	const char *description;
	// function pointer to the test function the test executes
	TestFunction execute;
	bool executed;
	TestResult testResult;
} TestCase;

/**
 * Stores information and status of a test suite
 * It also stores pointers to each individual test case.
 */
typedef struct TestSuite
{
	const char *name;
	TestCase **testCases;
	long testCaseSize;
	long testCount;
} TestSuite;

/**
 * Stores information about the status of the tests
 * Contains pointers to all suites
 */
typedef struct TestStatus
{
	TestSuite **testSuites;
	long testSuitesSize;
	long suiteCount;
} TestStatus;

// global test state
extern TestStatus *testStatus;

// Non documented functions
/**
 * Creates a TestCase element and initilizes its content
 */
TestCase * createTestCase(const char *testName, const char *testSuite, 
		const char *description, TestFunction testFunction);
/**
 * Frees all the allocated memory in a TestCase element
 */
void freeTestCase(TestCase *testCase);

/**
 * Creates a TestSuite elements and initializes its content
 */
TestSuite * createTestSuite(const char *name);
/**
 * Free all the allocated memory in a TestSuite element.
 * Frees all the TestCase elements inside the TestSuite.
 */ 
void freeTestSuite(TestSuite **testSuitePtr);

/**
 * Adds a TestCase to a TestSuite 
 */
bool addTestCase(TestSuite *testSuite, TestCase *testCase);
/**
 * Adds a TestSuite to the TestStatus object
 */
bool addTestSuite(TestStatus *testStatus, TestSuite *testSuite);

static void __attribute__((unused)) registerTestInSuite(TestSuite *testSuite, const char *testName,
		const char *description, TestFunction testFunction)
{
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot register test: testSuite is NULL\n");
		return;
	}
	TestCase *testCase = createTestCase(testName, testSuite->name, description, testFunction);
	if (!addTestCase(testSuite, testCase)) {
		freeTestCase(testCase);
		testCase = NULL;
	}
}

static inline TestSuite * __attribute__((unused)) findSuite(TestStatus *testStatus, const char *suiteName)
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
