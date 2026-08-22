/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../include/ctest_api.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
		fprintf(stderr, "[CTEST] | Error| actual: TRUE, expected: FALSE\n");
		exit(EXIT_FAILURE);
	}
}

