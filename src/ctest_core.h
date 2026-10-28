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
 * @brief Doxygen wrapper for __attribute__((unused)) guard
 */
#define CTEST_ATTRIBUTE_UNUSED __attribute__((unused))

/**
 * @defgroup ctest_internal CTest internal (non public) API
 *
 * @brief Library definitions, functions and global variables.
 *
 * @{
 */

/**
 * @brief Function pointer to the executable test function for each test
 */
typedef void (*CTest_TestFunction)(void);

/**
 * @brief Test execution result status
 */
typedef enum
{
	CTEST_SUCCESS,            /**< Successful execution */
	CTEST_FAILURE,            /**< Failed execution due to fail in assertion */
	CTEST_CRASH_SEGV,         /**< Execution crashed due to Segmentation Fault */
	CTEST_CRASH_OTHER         /**< Execution crashed due to other signal */
} CTest_TestExecutionStatus;

/**
 * @brief Test execution info, contains test execution status and logs (stderr, stdout)
 * written during the test execution
 */
typedef struct CTest_TestResult
{
	CTest_TestExecutionStatus status; /**< Status of the executed test */
	char *logs;                       /**< Test logs */
} CTest_TestResult;

/**
 * @brief Stores information and status of each test
 */
typedef struct CTest_TestCase
{
	const char *name;             /**< Test case name */
	const char *suiteName;        /**< Suite name of the suite the test case belongs to */
	const char *description;      /**< Description of the test case */
	const char *expectedError;    /**< Expected error of the test case, if expected to fail */
	bool errorIsRegexp;           /**< Flag that is enabled if the `expectedError` is a regular expression */
	bool executed;                /**< Flag that is enabled if a this test has ben executed */
	CTest_TestFunction execute;   /**< Function pointer to the test function the test executes */
	CTest_TestResult *testResult; /**< Test case execution result */
} CTest_TestCase;

/**
 * @brief Stores information and status of a test suite
 * It also stores pointers to each individual test case.
 */
typedef struct CTest_TestSuite
{
	const char *name;           /**< Test suite name */
	CTest_TestCase **testCases; /**< A list of the CTest_TestCase test cases registered in the suite */
	long testCaseSize;          /**< Size of the list CTest_TestSuite::testCases */
	long testCount;             /**< Number of tests registered in the suite */
	long successfulTests;       /**< Number of tests that executed successfully */
	long failedTests;           /**< Number of tests that have failed */
} CTest_TestSuite;

/**
 * @brief Stores information about the status of the tests
 * Contains pointers to all suites
 */
typedef struct CTest_TestStatus
{
	CTest_TestSuite **testSuites; /**< A list of the CTest_TestSuite test suites registered in the global test status */
	long testSuitesSize;          /**< Size of the list CTest_TestStatus::testSuites */
	long suiteCount;              /**< Number of suites registered in the global test status */
	long successfulTests;         /**< Number of tests that executed successfully */
	long failedTests;             /**< Number of tests that have failed */
} CTest_TestStatus;

/**
 * @brief Global test status, static per executable
 */
extern CTest_TestStatus *testStatus;

/**
 * @brief Creates a CTest_TestResult element and initializes its content
 * @param status global test execution status CTest_TestExecutionStatus
 * @param logs buffer with the logs that the test function produced
 * @param logSize size of the `logs` buffer
 * @return a pointer to a created CTest_TestResult
 */
CTest_TestResult *createTestResult(CTest_TestExecutionStatus status, const char *logs, size_t logSize);

/**
 * @brief Frees all the allocated memory in a CTest_TestResult element
 * @param testResult the CTest_TestResult to free
 */
void freeTestResult(CTest_TestResult **testResult);

/**
 * @brief Creates a CTest_TestCase element and initilizes its content
 * @param testName the test name
 * @param testSuite the test suite name which the test belongs to
 * @param description the description of the test
 * @param testFunction the test function that will be executed when executing the test
 * @return a new test case with the given parameters
 */
CTest_TestCase * createTestCase(const char *testName, const char *testSuite, 
		const char *description, CTest_TestFunction testFunction);
/**
 * @brief Frees all the allocated memory in a CTest_TestCase element
 * @param testCase the CTest_TestCase to free
 */
void freeTestCase(CTest_TestCase *testCase);

/**
 * @brief Creates a CTest_TestSuite elements and initializes its content
 * @param name the test suite name
 * @return a new initialized test suite
 */
CTest_TestSuite * createTestSuite(const char *name);
/**
 * @brief Free all the allocated memory in a CTest_TestSuite element.
 * Frees all the CTest_TestCase elements inside the CTest_TestSuite.
 * @param testSuitePtr the CTest_TestSuite to free
 */ 
void freeTestSuite(CTest_TestSuite **testSuitePtr);

/**
 * @brief Adds a CTest_TestCase to a CTest_TestSuite
 * @param testSuite the CTest_TestSuite that will contain the test case
 * @param testCase the CTest_TestCase that will be added to the test suite
 * @return `true` if the case has been successfully added, `false` otherwise
 */
bool addTestCase(CTest_TestSuite *testSuite, CTest_TestCase *testCase);

/**
 * @brief Adds a CTest_TestSuite to the CTest_TestStatus object
 * @param testStatus the global test status
 * @param testSuite the CTest_TestSuite that will be added to the global test status
 * @return `true` if the suite has been successfully added, `false` otherwise
 */
bool addTestSuite(CTest_TestStatus *testStatus, CTest_TestSuite *testSuite);

/**
 * @brief Creates and registers a CTest_TestCase into a CTest_TestSuite
 * @param testSuite suite name that will register the test case
 * @param testName test name that will registered in the suite
 * @param description test description
 * @param expectedError expected error of the test
 * @param errorIsRegexp `true` if the expected error is a regular expression, `false` otherwise
 * @param testFunction a CTest_TestFunction
 */
static void CTEST_ATTRIBUTE_UNUSED registerTestInSuite(CTest_TestSuite *testSuite, const char *testName,
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

/**
 * @brief finds a CTest_TestSuite inside the global test status CTest_TestStatus given by its name
 * @param testStatus global test status
 * @param suiteName name of the suite to find
 * @return a pointer to a CTest_TestSuite element with a name matching the `suiteName` , `NULL` if the suite is not find
 */
static inline CTest_TestSuite * CTEST_ATTRIBUTE_UNUSED findSuite(CTest_TestStatus *testStatus, const char *suiteName)
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

/** @} */
#endif // __CTEST_CORE_H
