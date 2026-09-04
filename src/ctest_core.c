/**
 * Game Boy emulator
 * @Author Miguel Blanco Godón
 */

#include "ctest_core.h"

#include <asm/termbits.h>
#include <asm/termios.h>
#include <errno.h>
#include <execinfo.h>
#include <getopt.h>
#include <regex.h>
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
	testResult->logs   = NULL;
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
		if ((*testResult)->logs != NULL) {
			free((*testResult)->logs);
			(*testResult)->logs = NULL;
		}
		free(*testResult);
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
	testCase->expectedError = NULL;
	testCase->execute = testFunction;
	testCase->executed = false;
	testCase->testResult = NULL;
	return testCase;
}

void freeTestCase(TestCase *testCase)
{
	if (testCase != NULL) {
		if (testCase->testResult != NULL) {
			freeTestResult(&testCase->testResult);
		}
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

TestResult * getTestResultFromExpectedFailure(TestExecutionStatus status, const char *expected,
		const char *output, int size, bool isRegexp)
{
	const char *regularizedExpected = expected;
	regex_t regexp;
	if (isRegexp) {
		if (regcomp(&regexp, expected, 0) != 0) {
			int bufferSize = size + 100;
			char buffer[size + 100];
			snprintf(buffer, bufferSize, "[CTEST] | Error | Unable to create regular expression for: '%s'\n", expected);
			regfree(&regexp);
			return createTestResult(CRASH_OTHER, buffer, bufferSize);
		} else {
			if (regexec(&regexp, output, 0, NULL, 0) == 0) {
				regfree(&regexp);
				return createTestResult(SUCCESS, output, size);
			} else {
				int bufferSize = size + 100;
				char buffer[size + 100];
				snprintf(buffer, bufferSize, "[CTEST] | Error | regexp does not match output | regexp: '%s', output: '%s'\n", expected, output);
				regfree(&regexp);
				return createTestResult(FAILURE, buffer, bufferSize);
			}
		}
	}
	// if we do not expect failure or the failure does not match the expected, return test failure
	// must treat empty string differently as will match any string
	if (expected == NULL || strstr(output, regularizedExpected) == NULL) {
		return createTestResult(status, output, size);
	} else {
		// if it matches, return success
		return createTestResult(SUCCESS, output, size);
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
TestResult * executeTest(TestFunction testFunction, const char *expectedError, bool regexp)
{
	pid_t pid;
	int status;
	int pipefd[2];
	const int bufferSize = 65535;
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
	// flush buffers to child process has 'clean' output and error buffers
	fflush(stdout); fflush(stderr);
	if (pid == 0) {
		// child process is write only
		close(pipefd[0]);
		// close read end and redirect output and error output
		dup2(pipefd[1], STDOUT_FILENO);
		dup2(pipefd[1], STDERR_FILENO);
		close(pipefd[1]);
		testFunction();
		_Exit(EXIT_SUCCESS);
	}
	// parent process is read only
	close(pipefd[1]);

	waitpid(pid, &status, 0);
	// TODO read output from child process and store as test info (show on failure)
	// temporary read in a temporary buffer and discard
	bytesRead = read(pipefd[0], childProcessOutput, bufferSize - 1);
	close(pipefd[0]);
	if (WIFEXITED(status)) {
		return WEXITSTATUS(status) == EXIT_SUCCESS ?
		   createTestResult(SUCCESS, childProcessOutput, bytesRead) :
		   getTestResultFromExpectedFailure(FAILURE, expectedError, childProcessOutput, bytesRead, regexp);
	}
	if (WIFSIGNALED(status)) {
		if (WTERMSIG(status) == SIGSEGV) {
			return getTestResultFromExpectedFailure(CRASH_SEGV, expectedError, childProcessOutput, bytesRead, regexp);
		}
	}
	return getTestResultFromExpectedFailure(CRASH_OTHER, expectedError, childProcessOutput, bytesRead, regexp);
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
void executeTests(TestStatus *testStatus, const char *suiteName, const char *testName)
{
	struct timeval tInit, tFinish, singleTestStart, singleTestFinish;
	double elapsedTime = 0, singleTestExecutionElapsedTime = 0;
	long totalTests = 0;
	long successfulTests = 0;
	long failedTests = 0;
	long suiteSuccessfulTests;
	long suiteFailedTests;
	int terminalWidth = getTerminalWidth();
	gettimeofday(&tInit, NULL);
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		totalTests += testStatus->testSuites[i]->testCount;
	}
	printf("CTest v%s (%ld suites, %ld tests) \n\n", CTEST_VERSION, testStatus->suiteCount, totalTests);
	for (long i = 0; i < testStatus->suiteCount; ++i) {
		if (suiteName != NULL && strcmp(suiteName, testStatus->testSuites[i]->name) != 0) {
			continue;
		}
		printSuiteName(testStatus->testSuites[i]->name, terminalWidth);
		suiteFailedTests = 0;
		suiteSuccessfulTests = 0;
		for (long j = 0; j < testStatus->testSuites[i]->testCount; ++j) {
			if (testName != NULL && strcmp(testName, testStatus->testSuites[i]->testCases[j]->name) != 0) {
				continue;
			}
			gettimeofday(&singleTestStart, NULL);
			testStatus->testSuites[i]->testCases[j]->testResult =
				executeTest(testStatus->testSuites[i]->testCases[j]->execute,
						testStatus->testSuites[i]->testCases[j]->expectedError,
						testStatus->testSuites[i]->testCases[j]->errorIsRegexp);
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

void printStackTrace(void)
{
	void *stackTraceBuffer[10];
	int stackTraceBufferCount = 0;
	char **stackTrace = NULL;
	stackTraceBufferCount = backtrace(stackTraceBuffer, 10);
	stackTrace = backtrace_symbols(stackTraceBuffer, stackTraceBufferCount);
	if (stackTrace != NULL) {
		fprintf(stderr, "Stack trace (%d):\n", stackTraceBufferCount - 2);
		for (int i = 2; i < stackTraceBufferCount; ++i) {
			// add padding to the stack trace to ease visualization
			for (int j = 1; j < i; j++) {
				fprintf(stderr, "   ");
			}
			fprintf(stderr, "[%d] %s\n", stackTraceBufferCount - i, stackTrace[i]);
		}
	}
	free(stackTrace);
}

/**
 * Handles each signal raised.
 * Only a handful of 'important' signals are treated specifically
 * Most signals are treated in a default 'just print the error' manner
 */
void handleSignal(int signal)
{
	const char *sigName = strsignal(signal);
	const char *errorTemplate = "[CTEST] | Error | Received %s (%s):\n";
	switch (signal)
	{
		case SIGABRT:
			fprintf(stderr, errorTemplate, sigName, "SIGABRT");
			break;
		case SIGALRM:
			fprintf(stderr, errorTemplate, "timeout", "SIGALRM");
			break;
		case SIGBUS:
			fprintf(stderr, errorTemplate, sigName, "SIGBUS");
			break;
		case SIGCHLD:
			// pass in this case as each forked process raises this when the child process ends
			return;
		case SIGFPE:
			fprintf(stderr, errorTemplate, sigName, "SIGFPE");
			break;
		case SIGILL:
			fprintf(stderr, errorTemplate, sigName, "SIGILL");
			break;
		case SIGINT:
			fprintf(stderr, errorTemplate, sigName, "SIGINT");
			break;
		case SIGSEGV:
			fprintf(stderr, errorTemplate, sigName, "SIGSEGV");
			break;
		default:
			fprintf(stderr, "[CTEST] | Error | Received signal '%s':\n", sigName);
	};
	printStackTrace();
	_Exit(signal);
}

/**
 * Registers signal handlers for catching signals occurred during testing
 * As in most architectures signals are ints in the range 1-31, the first
 * 31 positive integers are registered in an optimistic approach
 */
void registerSignalHandlers()
{
	for (int i = 1; i < 32; ++i) {
		signal(i, handleSignal);
	}
}

typedef struct InputParameters
{
	bool invalidOption;
	bool version;
	bool help;
	char *testSuite;
	char *testCase;
} InputParameters;

void processInputParameters(InputParameters *inputParameters, int argc, char **argv)
{
	const char *shortOptions = "c:hs:v";
	static struct option longOptions[] =
	{
		{"case", 1, NULL, 'c'},
		{"help", 0, NULL, 'h'},
		{"suite", 1, NULL, 's'},
		{"version", 0, NULL, 'v'}
	};
	int optionIndex = 0;
	inputParameters->invalidOption = false;
	inputParameters->version = false;
	inputParameters->help = false;
	inputParameters->testSuite = NULL;
	inputParameters->testCase = NULL;
	int c = getopt_long(argc, argv, shortOptions, longOptions, &optionIndex);

	if (c == -1) {
		return;
	}
	switch (c) {
		case 'h':
			inputParameters->help = true;
			break;
		case 'c':
			inputParameters->testCase = optarg;
			// getopt does only return the first occurrency, we can get the others using
			// the optind variable
			if (optind + 1 < argc) {
				if (strcmp("-s", argv[optind]) == 0 || strcmp("--suite", argv[optind]) == 0) {
					inputParameters->testSuite = argv[optind + 1];
				}
			}
			break;
		case 's':
			inputParameters->testSuite = optarg;
			// same as in the previous block
			if (optind + 1 < argc) {
				if (strcmp("-c", argv[optind]) == 0 || strcmp("--case", argv[optind]) == 0) {
					inputParameters->testCase = argv[optind + 1];
				}
			}
			break;
		case 'v':
			inputParameters->version = true;
			break;
		default:
			inputParameters->invalidOption = true;
	}
}

void printUsage(void)
{
	printf("Usage: <ctest_executable> [OPTIONS]\n\n");
    printf("Options:\n");
    printf("  -h, --help                            Show this help message and exit\n");
    printf("  -v, --version                         Show program version\n");
    printf("  -s, --suite <name>                    Execute test suite <param>\n");
    printf("  -s, --suite <name> -c, --case <name>  Execute test case <param2> from suite <param>\n");
}

/**
 * Framework entrypoint.
 */
int __attribute__((weak)) main(int argc, char **argv)
{
	InputParameters inputParameters;
	processInputParameters(&inputParameters, argc, argv);
	if (inputParameters.invalidOption) {
		printUsage();
		return EXIT_FAILURE;
	}
	if (inputParameters.help) {
		printUsage();
		return EXIT_SUCCESS;
	}
	if (inputParameters.version) {
		printCTestLogo();
		return EXIT_SUCCESS;
	}
	if (inputParameters.testCase != NULL && inputParameters.testSuite == NULL) {
		fprintf(stderr, "[CTEST] | Error | test case filtering must be acompanied of suite filtering\n");
		printUsage();
		return EXIT_FAILURE;
	}
	registerSignalHandlers();
	executeTests(testStatus, inputParameters.testSuite, inputParameters.testCase);
	return EXIT_SUCCESS;
}
