/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../include/ctest_api.h"

#include <execinfo.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printCallerStackTrace(void)
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

void endTestWithError(const char *filePath, int line, const char *caller, const char *error)
{
	fprintf(stderr, "[CTEST] Assertion error in '%s:%d' | %s | %s\n", filePath, line, caller, error);
	printCallerStackTrace();
	// flush buffers to avoid data loss through the child - parent pipe
	fflush(stdout); fflush(stderr);
	_Exit(EXIT_FAILURE);
}

void assertTrue(const char *filePath, int line, bool condition)
{
	if (!condition) {
		endTestWithError(filePath, line, __func__, "actual: FALSE, expected: TRUE");
	}
}

void assertFalse(const char *filePath, int line, bool condition)
{
	if (condition) {
		endTestWithError(filePath, line, __func__, "actual: TRUE, expected: FALSE");
	}
}

void assertNull(const char *filePath, int line, void *pointer)
{
	if (pointer != NULL) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %p, expected: NULL", pointer);
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void assertNonNull(const char *filePath, int line, void *pointer)
{
	if (pointer == NULL) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: NULL, expected: %p", pointer);
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void assertLongEquals(const char *filePath, int line, long expected, long actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %ld, expected: %ld", actual, expected);
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void assertStringEquals(const char *filePath, int line, const char *expected, const char *actual)
{
	char errorBuffer[64];
	snprintf(errorBuffer, 64, "actual: %s, expected: %s", actual, expected);
	if (expected == NULL && actual == NULL) {
		return;
	}
	if (expected == NULL && actual != NULL) {
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
	if (expected != NULL && actual == NULL) {
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
	if (strcmp(expected, actual) != 0) {
		endTestWithError(filePath, line, __func__, errorBuffer);
	}
}
