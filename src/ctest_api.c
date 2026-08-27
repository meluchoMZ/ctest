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

void endTestWithError(const char *file, int line, const char *caller, const char *error)
{
	fprintf(stderr, "[CTEST] Assertion error in '%s:%d' | %s | %s\n", file, line, caller, error);
	printCallerStackTrace();
	// flush buffers to avoid data loss through the child - parent pipe
	fflush(stdout); fflush(stderr);
	_Exit(EXIT_FAILURE);
}

void assertTrue(bool condition)
{
	if (!condition) {
		fprintf(stderr, "[CTEST] | Error | actual: FALSE, expected: TRUE\n");
		_Exit(EXIT_FAILURE);
	}
}

void assertFalse(bool condition)
{
	if (condition) {
		fprintf(stderr, "[CTEST] | Error | actual: TRUE, expected: FALSE\n");
		_Exit(EXIT_FAILURE);
	}
}

void assertNull(void *pointer)
{
	if (pointer != NULL) {
		fprintf(stderr, "[CTEST] | Error | assertNull: actual: %p, expected: NULL\n", pointer);
		_Exit(EXIT_FAILURE);
	}
}

void assertNonNull(void *pointer)
{
	if (pointer == NULL) {
		fprintf(stderr, "[CTEST] | Error | assertNonNull: actual: NULL, expected: %p\n", pointer);
		_Exit(EXIT_FAILURE);
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

void assertStringEquals(const char *expected, const char *actual)
{
	if (expected == NULL && actual == NULL) {
		return;
	}
	if (expected == NULL && actual != NULL) {
		_Exit(EXIT_FAILURE);
	}
	if (expected != NULL && actual == NULL) {
		_Exit(EXIT_FAILURE);
	}
	if (strcmp(expected, actual) != 0) {
		_Exit(EXIT_FAILURE);
	}
}
