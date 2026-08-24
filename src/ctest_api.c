/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../include/ctest_api.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void assertTrue(bool condition)
{
	if (!condition) {
		fprintf(stderr, "[CTEST] | Error | actual: FALSE, expected: TRUE\n");
		exit(EXIT_FAILURE);
	}
}

void assertFalse(bool condition)
{
	if (condition) {
		fprintf(stderr, "[CTEST] | Error | actual: TRUE, expected: FALSE\n");
		exit(EXIT_FAILURE);
	}
}

void assertNull(void *pointer)
{
	if (pointer != NULL) {
		fprintf(stderr, "[CTEST] | Error | assertNull: actual: %p, expected: NULL\n", pointer);
		exit(EXIT_FAILURE);
	}
}

void assertNonNull(void *pointer)
{
	if (pointer == NULL) {
		fprintf(stderr, "[CTEST] | Error | assertNonNull: actual: NULL, expected: %p\n", pointer);
		exit(EXIT_FAILURE);
	}
}

void assertLongEquals(long expected, long actual)
{
	if (expected != actual) {
		fprintf(stderr, "[CTEST] | Error | actual: %ld, expected: %ld\n", actual, expected);
		exit(EXIT_FAILURE);
	}
}

void assertStringEquals(const char *expected, const char *actual)
{
	if (expected == NULL && actual == NULL) {
		return;
	}
	if (expected == NULL && actual != NULL) {
		exit(EXIT_FAILURE);
	}
	if (expected != NULL && actual == NULL) {
		exit(EXIT_FAILURE);
	}
	if (strcmp(expected, actual) != 0) {
		exit(EXIT_FAILURE);
	}
}
