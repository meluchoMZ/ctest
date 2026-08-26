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
#include <sys/time.h>
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

TestResult * createTestResult(TestExecutionStatus status, const char *logs, size_t logSize)
{
	TestResult *testResult = malloc(sizeof(TestResult));
	if (testResult == NULL) {
		fprintf(stderr, "[CTEST] | Error | could not allocate memory for TestResult element: %s\n",
				strerror(errno));
		return NULL;
	}
	testResult->status = status;
	if (logSize > 0 && logs != NULL) {
		testResult->logs = malloc(logSize + 1 * sizeof(char));
		if (testResult->logs == NULL) {
			fprintf(stderr, "[CTEST] | Error | could not allocate memory for TestResult.logs: %s\n",
					strerror(errno));
			return testResult;
		}
		for (size_t i = 0; i < logSize; ++i) {
			testResult->logs[i] = logs[i];
		}
		testResult->logs[logSize] = '\0';
	}
	return testResult;
}

void freeTestResult(TestResult **testResult)
{
	if (testResult != NULL && *testResult != NULL) {
		free((*testResult)->logs);
		(*testResult)->logs = NULL;
		*testResult = NULL;
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
	return testCase;
}

void freeTestCase(TestCase *testCase)
{
	if (testCase != NULL) {
		freeTestResult(&testCase->testResult);
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
		if (strcmp(testCase->name, testSuite->testCases[i]->name) == 0) {
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
 * Also redirects stdout and stderr to the parent process through
 * a pipe so that the code executed in the test does not get
 * shown to the user
 */
TestResult * executeTest(TestFunction testFunction)
{
	pid_t pid;
	int status;
	int pipefd[2];
	const int bufferSize = 4096;
	char childProcessOutput[bufferSize];
	size_t bytesRead = 0;

	if (pipe(pipefd) != 0) {
		fprintf(stderr, "[CTEST] | Error | could not create pipe: %s\n", strerror(errno));
		return createTestResult(CRASH_OTHER, NULL, 0);
	}
	pid = fork();
	if (pid == -1) {
		close(pipefd[0]);
		close(pipefd[1]);
		fprintf(stderr, "[CTEST] | Error | could not create child process: %s\n", strerror(errno));
		return createTestResult(CRASH_OTHER, NULL, 0);
	}
	if (pid == 0) {
		// child process is write only
		close(pipefd[0]);
		// close read end and redirect output and error output
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);
		testFunction();
		exit(EXIT_SUCCESS);
	}
	// parent process is read only
	close(pipefd[1]);
	// TODO read output from child process and store as test info (show on failure)
	// temporary read in a temporary buffer and discard
	bytesRead = read(pipefd[0], childProcessOutput, bufferSize - 1);
	close(pipefd[0]);

	waitpid(pid, &status, 0);
	if (WIFEXITED(status)) {
	return WEXITSTATUS(status) == EXIT_SUCCESS ?
	   createTestResult(SUCCESS, childProcessOutput, bytesRead) :
	   createTestResult(FAILURE, childProcessOutput, bytesRead);
	}
	if (WIFSIGNALED(status)) {
		if (WTERMSIG(status) == SIGSEGV) {
			return createTestResult(CRASH_SEGV, childProcessOutput, bytesRead);
		}
	}
	return createTestResult(CRASH_OTHER, childProcessOutput, bytesRead);
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
 * Prints a single test execution with elapsed time
 */
void printTestResult(const char *testCaseName, int terminalWidth, double testExecutionTime, TestResult *testResult)
{
	size_t remainingColumns = terminalWidth - strlen(testCaseName) - TEST_RESULT_MESSAGE_SIZE - 6; // 6 cols padding
	const char *statusSymbol = testResult->status == SUCCESS
		? GREEN "✓" RESET
		: RED "✗" RESET;
	char testExecutionStringBuffer[32];
	int testExecutionStringSize = snprintf(testExecutionStringBuffer,
			sizeof(testExecutionStringBuffer), "%.4f ms", testExecutionTime);
	remainingColumns -= testExecutionStringSize;
	printf("    %s %s", statusSymbol, testCaseName);
	if (remainingColumns > 0) {
		for (size_t i = 0; i < remainingColumns; ++i) {
			putchar(' ');
		}
	}
	printf("%s\n", testExecutionStringBuffer);
	// print all child process STDOUT and STDERR logs as errors
	if (testResult->logs != NULL && testResult->status != SUCCESS) {
		printf(RED "%s\n" RESET, testResult->logs);
	}
}

void printSuiteName(const char *name,int terminalWidth)
{
	size_t remainingColumns = terminalWidth - strlen(name) - 5; // 5 columns of padding + prefix
	printf("--- %s ", name);
	if (remainingColumns > 0) {
		for (size_t i = 0; i < remainingColumns; ++i) {
			putchar('-');
		}
	}
	printf("\n");
}

double computeElapsedTimeInSeconds(struct timeval start, struct timeval finish)
{
	double elapsedTime = (finish.tv_sec - start.tv_sec) +
		((finish.tv_usec - start.tv_usec) / (double) 1000000);
	return elapsedTime;
}

double computeElapsedTimeInMiliseconds(struct timeval start, struct timeval finish)
{
	double elapsedTime = ((finish.tv_sec - start.tv_sec) * (double) 1000) +
		((finish.tv_usec - start.tv_usec) / (double) 1000);
	return elapsedTime;
}

/**
 * Executes all the tests registered in a TestStatus element
 */
void executeTests(TestStatus *testStatus)
{
	struct timeval tInit, tFinish, singleTestStart, singleTestFinish;
	double elapsedTime = 0, singleTestExecutionElapsedTime = 0;
	long totalTests = 0;
	long successfulTests = 0;
	long failedTests = 0;
	long suiteSuccessfulTests;
	long suiteFailedTests;
	int terminalWidth = getTerminalWidth();
	printCTestLogo();
	gettimeofday(&tInit, NULL);
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		totalTests += testStatus->testSuites[i]->testCount;
	}
	printf("CTest v%s (%ld suites, %ld tests) \n\n", CTEST_VERSION, testStatus->suiteCount, totalTests);
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		printSuiteName(testStatus->testSuites[i]->name, terminalWidth);
		suiteFailedTests = 0;
		suiteSuccessfulTests = 0;
		for (long j = 0; j < testStatus->testSuites[i]->testCount; ++j) {
			gettimeofday(&singleTestStart, NULL);
			testStatus->testSuites[i]->testCases[j]->testResult =
				executeTest(testStatus->testSuites[i]->testCases[j]->execute);
			gettimeofday(&singleTestFinish, NULL);
			singleTestExecutionElapsedTime = computeElapsedTimeInMiliseconds(singleTestStart, singleTestFinish);
			testStatus->testSuites[i]->testCases[j]->executed = true;
			testStatus->testSuites[i]->testCases[j]->testResult->status == SUCCESS ?
				suiteSuccessfulTests++ : suiteFailedTests++;
			printTestResult(testStatus->testSuites[i]->testCases[j]->name, terminalWidth,
					singleTestExecutionElapsedTime, testStatus->testSuites[i]->testCases[j]->testResult);
		}
		successfulTests += suiteSuccessfulTests;
		failedTests += suiteFailedTests;
		printf("    PASS: [%ld/%ld]\n\n", suiteSuccessfulTests, suiteSuccessfulTests + suiteFailedTests);
	}
	gettimeofday(&tFinish, NULL);
	elapsedTime = computeElapsedTimeInSeconds(tInit, tFinish);
	for (int i = 0; i < terminalWidth; ++i) {
		putchar('-');
	}
	if (elapsedTime < 1) {
		printf("\n\nRESULTS: " GREEN "%ld" RESET " passed, " RED "%ld" RESET " failed in %.4f ms\n", successfulTests, failedTests, elapsedTime * (double) 1000);
	} else {
		printf("\n\nRESULTS: " GREEN "%ld" RESET " passed, " RED "%ld" RESET " failed in %.4f s\n", successfulTests, failedTests, elapsedTime);
	}
	if (failedTests == 0) {
		printf("STATUS:  " GREEN "SUCCESS" RESET "\n");
	} else {
		printf("STATUS:  " RED "FAILURE" RESET "\n");
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
