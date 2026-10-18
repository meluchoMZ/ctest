/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../include/ctest_api.h"

#include <execinfo.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CT_printCallerStackTrace(void)
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

void CT_endTestWithError(const char *filePath, int line, const char *caller, const char *error)
{
	fprintf(stderr, "[CTEST] Assertion error in '%s:%d' | %s | %s\n", filePath, line, caller, error);
	CT_printCallerStackTrace();
	// flush buffers to avoid data loss through the child - parent pipe
	fflush(stdout); fflush(stderr);
	_Exit(EXIT_FAILURE);
}

void CT_AssertTrue(const char *filePath, int line, bool condition)
{
	if (!condition) {
		CT_endTestWithError(filePath, line, __func__, "actual: FALSE, expected: TRUE");
	}
}

void CT_AssertFalse(const char *filePath, int line, bool condition)
{
	if (condition) {
		CT_endTestWithError(filePath, line, __func__, "actual: TRUE, expected: FALSE");
	}
}

void CT_AssertNull(const char *filePath, int line, void *pointer)
{
	if (pointer != NULL) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %p, expected: NULL", pointer);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_AssertNonNull(const char *filePath, int line, void *pointer)
{
	if (pointer == NULL) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: NULL, expected: %p", pointer);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_AssertPointerEquals(const char *filePath, int line, void *expected, void *actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %p, expected: %p", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert8BitUIntEquals(const char *filePath, int line, uint8_t expected, uint8_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert16BitUIntEquals(const char *filePath, int line, uint16_t expected, uint16_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert32BitUIntEquals(const char *filePath, int line, uint32_t expected, uint32_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert64BitUIntEquals(const char *filePath, int line, uint64_t expected, uint64_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %" PRIu64 ", expected: %" PRIu64, actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert8BitIntEquals(const char *filePath, int line, int8_t expected, int8_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert16BitIntEquals(const char *filePath, int line, int16_t expected, int16_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert32BitIntEquals(const char *filePath, int line, int32_t expected, int32_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %d, expected: %d", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_Assert64BitIntEquals(const char *filePath, int line, int64_t expected, int64_t actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %" PRIi64 ", expected: %" PRIi64, actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_AssertIntEquals(const char *filePath, int line, long expected, long actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %ld, expected: %ld", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_AssertLongEquals(const char *filePath, int line, long expected, long actual)
{
	if (expected != actual) {
		char errorBuffer[64];
		snprintf(errorBuffer, 64, "actual: %ld, expected: %ld", actual, expected);
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}

void CT_AssertStringEquals(const char *filePath, int line, const char *expected, const char *actual)
{
	char errorBuffer[64];
	snprintf(errorBuffer, 64, "actual: %s, expected: %s", actual, expected);
	if (expected == NULL && actual == NULL) {
		return;
	}
	if (expected == NULL && actual != NULL) {
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
	if (expected != NULL && actual == NULL) {
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
	if (strcmp(expected, actual) != 0) {
		CT_endTestWithError(filePath, line, __func__, errorBuffer);
	}
}
