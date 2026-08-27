/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#ifndef __CTEST_API_H
#define __CTEST_API_H

#include "ctest_core.h"

#include <stdint.h>

// Macros for test declarations
#define TEST(suite, name, description) \
	static void __attribute__((unused)) suite##_##name##_impl(void); \
	static void __attribute__((constructor)) suite##_##name##_register(void) \
		{ \
		TestSuite *testSuite = findSuite(testStatus, #suite); \
		if (testSuite == NULL) { \
			TestSuite *testSuite = createTestSuite(#suite); \
			if (!addTestSuite(testStatus, testSuite)) { \
				freeTestSuite(&testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)

// Public documented test functions

/**
 * Tests that the given `condition` is `true`
 */
void assertTrue(bool condition);

/**
 * Tests that the given `condition` is `true`
 */
void assertFalse(bool condition);

/**
 * Tests that the given pointer is `NULL`
 */
void assertNull(void *pointer);

/**
 * Tests that the given pointer is not `NULL`
 */
void assertNonNull(void *pointer);

/**
 * Tests that the given 8 bit integers have the same value
 */
void assert8BitIntEquals(uint8_t expected, uint8_t actual);

/**
 * Tests that the given 16 bit integers have the same value
 */
void assert16BitIntEquals(uint16_t expected, uint16_t actual);

/**
 * Tests that the given longs have the same value
 */
#define ASSERT_LONG_EQUALS(expected, actual) \
	assertLongEquals(__FILE__, __LINE__, expected, actual)

void assertLongEquals(const char *filePath, int line, long expected, long actual);

/**
 * Tests that the given strings are similar
 */
void assertStringEquals(const char *expected, const char *actual);

#endif // __CTEST_API_H
