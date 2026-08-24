/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "ctest_core.h"

#include <asm/termbits.h>
#include <asm/termios.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <unistd.h>

TestStatus *testStatus = NULL;
// Start of global test state
static void __attribute__((constructor(101))) initTests(void)
{
	testStatus = malloc(sizeof(TestStatus));
	if (testStatus == NULL) {
		fprintf(stderr, "[CTEST] Error initializing test framework: %s\n", strerror(errno));
		return;
	}
	testStatus->testSuites = malloc(sizeof(TestSuite) * 10);
	if (testStatus->testSuites == NULL) {
		fprintf(stderr, "[CTEST] | Error | could not allocate memory for storing test suites: %s\n", strerror(errno));
		free(testStatus);
		testStatus = NULL;
		return;
	}
	testStatus->testSuitesSize = 10;
	testStatus->suiteCount = 0;
}

// After test, free all the test memory usage
static void __attribute__((destructor)) finalizeTests(void)
{
	if (testStatus != NULL) {
		for (long i = testStatus->suiteCount - 1; i >= 0; --i) {
			// freeTestSuite do already frees its test cases
			freeTestSuite(&testStatus->testSuites[i]);
		}
		free(testStatus->testSuites);
		testStatus->testSuites = NULL;
		free(testStatus);
		testStatus = NULL;
	}
}	

TestCase * createTestCase(const char *testName, const char *testSuite, const char *description, 
		TestFunction testFunction)
{
	TestCase *testCase = malloc(sizeof(TestCase));
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] Error: could not allocate memory for `TestCase` element: %s\n", strerror(errno));
	}
	testCase->name = testName;
	testCase->suiteName = testSuite;
	testCase->description = description;
	testCase->execute = testFunction;
	testCase->executed = false;
	testCase->successful = false;
	return testCase;
}

void freeTestCase(TestCase *testCase)
{
	if (testCase != NULL) {
		free(testCase);
		testCase = NULL;
	}
}


bool addTestCase(TestSuite *testSuite, TestCase *testCase)
{
	if (testCase == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestCase to TestSuite element: testCase == NULL\n");
		return false;
	}
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestCase to TestSuite element: testSuite == NULL\n");
		return false;
	}
	if (testSuite->testCases == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add testCase to TestSuite element: testSuite->testCases == NULL\n");
		return false;
	}
	if (testCase->name == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestCase to TestSuite element: testCase->name == NULL\n");
		return false;
	}

	for (int i = 0; i < testSuite->testCount; ++i) {
		if (strcmp(testSuite->name, testSuite->testCases[i]->name) == 0) {
			fprintf(stderr, "[CTEST] | Error | cannot add TestCase '%s': test case already exists\n", testSuite->name);
			return false;
		}
	}

	// realloc if necessary
	if (testSuite->testCount == testSuite->testCaseSize) {
		long newSize = testSuite->testCaseSize + 10;
		TestCase **tmp = realloc(testSuite->testCases, newSize * sizeof(TestCase));
		if (tmp == NULL) {
			fprintf(stderr, "[CTEST] | Error | could not realloc more size for the TestCases list: %s\n", strerror(errno));
			return false;
		} else {
			testSuite->testCases = tmp;
			testSuite->testCaseSize += 10;
		}
	}
	
	testSuite->testCases[testSuite->testCount++] = testCase;
	return true;
}

TestSuite * createTestSuite(const char *name)
{
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | could not allocate memory for `TestSuite` element: %s\n", strerror(errno));
		return NULL;
	}
	testSuite->name = name;
	testSuite->testCases = malloc(sizeof(TestCase) * 10);
	if (testSuite->testCases == NULL) {
		fprintf(stderr, "[CTEST] | Error | could not allocate memory for test cases in suite '%s': %s\n", 
				name, strerror(errno));
		free(testSuite);
		testSuite = NULL;
		return NULL;
	}
	testSuite->testCaseSize = 10;
	testSuite->testCount = 0;
	return testSuite;
}

bool addTestSuite(TestStatus *testStatus, TestSuite *testSuite)
{
	if (testStatus == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestSuite to TestStatus element: testStatus == NULL\n");
		return false;
	}
	if (testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestSuite to TestStatus element: testSuite == NULL\n");
		return false;
	}
	if (testStatus->testSuites == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestSuite to TestStatus element: testSuite->testSuites == NULL\n");
		return false;
	}
	if (testSuite->name == NULL) {
		fprintf(stderr, "[CTEST] | Error | cannot add TestSuite to TestStatus element: testSuite->name == NULL\n");
		return false;
	}

	for (int i = 0; i < testStatus->suiteCount; ++i) {
		if (strcmp(testSuite->name, testStatus->testSuites[i]->name) == 0) {
			fprintf(stderr, "[CTEST] | Error | cannot add TestSuite '%s': suite already exists\n", testSuite->name);
			return false;
		}
	}

	// realloc if necessary
	if (testStatus->suiteCount == testStatus->testSuitesSize) {
		long newSize = testStatus->suiteCount + 10;
		TestSuite **tmp = realloc(testStatus->testSuites, newSize * sizeof(TestSuite));
		if (tmp == NULL) {
			fprintf(stderr, "[CTEST] | Error | could not realloc more size for the TestSuites lists: %s\n", strerror(errno));
			return false;
		} else {
			testStatus->testSuites = tmp;
			testStatus->testSuitesSize += 10;
		}
	}
	
	testStatus->testSuites[testStatus->suiteCount++] = testSuite;
	return true;
}

void freeTestSuite(TestSuite **testSuitePtr)
{
	if (testSuitePtr != NULL) {
		TestSuite *testSuite = *testSuitePtr;
		if (testSuite != NULL) {
			if (testSuite->testCases != NULL) {
				for (int i = 0; i < testSuite->testCount; ++i) {
					freeTestCase(testSuite->testCases[i]);
					testSuite->testCases[i] = NULL;
				}
				free(testSuite->testCases);
				testSuite->testCases = NULL;
			}
			free(testSuite);
			testSuite = NULL;
			*testSuitePtr = NULL;
		}
	}
}

/**
 * Executes a single test function and stores its result.
 * Each test function has to be executed inside a child process, 
 * in order to be able to capture and store failures, especially
 * the ones related to signasl.
 */
TestResult executeTest(TestFunction testFunction)
{
	pid_t pid;
	int status;
	pid = fork();
	if (pid == -1) {
		fprintf(stderr, "[CTEST] | Error | could not create child process: %s\n", strerror(errno));
		return CRASH_OTHER;
	}
	if (pid == 0) {
		testFunction();
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) {
	return WEXITSTATUS(status) == EXIT_SUCCESS ? SUCCESS : FAILURE;
	}
	if (WIFSIGNALED(status)) {
		if (WTERMSIG(status) == SIGSEGV) {
			return CRASH_SEGV;
		}
	}
	return CRASH_OTHER;	
}

/**
 * Prints in real time with progress bars test execution and status
 */
void testPrinter(void)
{
}

/**
 * Prints CTest logo, version and author copyright in ascii art
 */
void printCTestLogo(void)
{
	const char *logo =
		"  ____ _____         _   \n"
		" / ___|_   _|__  ___| |_ \n"
		"| |     | |/ _ \\/ __| __|\n"
		"| |___  | |  __/\\__ \\ |_ \n"
		" \\____| |_|\\___||___/\\__|\n";
	printf("%s\n", logo);
	printf("CTest Version: %s\n", CTEST_VERSION);
	printf("%s\n\n\n", CTEST_COPYRIGHT);
}

/**
 * Obtains number of columns from the terminal CTest is being run
 */
int getTerminalWidth()
{
	struct winsize terminalWindowSize;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalWindowSize) != 0) {
		fprintf(stderr, "[CTEST] | Error | could not get terminal window size: %s\n", strerror(errno));
		return -1;
	}
	return terminalWindowSize.ws_col;
}

/**
 * Prints a test name followed by dots and end with [SUCESS] or [FAILURE]
 */
void printTestResult(const char *testCaseName, int terminalWidth, bool success)
{
	size_t remainingColumns = terminalWidth - strlen(testCaseName) - TEST_RESULT_MESSAGE_SIZE;
	printf("%s", testCaseName);
	if (remainingColumns > 0) {
		for (size_t i = 0; i < remainingColumns; ++i) {
			putchar('.');
		}
	}
	printf("%s\n", success ?
			GREEN SUCCESS_MESSAGE RESET :
			RED FAILURE_MESSAGE RESET);
}

/**
 * Executes all the tests registered in a TestStatus element
 */
void executeTests(TestStatus *testStatus)
{
	int terminalWidth = getTerminalWidth();
	printCTestLogo();
	printf("Found '%ld' test suites\n", testStatus->suiteCount);
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		printf("Executing test suite %s. Tests found: %ld\n", testStatus->testSuites[i]->name,
				testStatus->testSuites[i]->testCount);
		for (long j = 0; j < testStatus->testSuites[i]->testCount; ++j) {
			TestResult result = executeTest(testStatus->testSuites[i]->testCases[j]->execute);
			printTestResult(testStatus->testSuites[i]->testCases[j]->name, terminalWidth, result == SUCCESS);
		}
	}
}

/**
 * Framework entrypoint.
 */
int __attribute__((weak)) main(void)
{
	executeTests(testStatus);
	return EXIT_SUCCESS;
}
