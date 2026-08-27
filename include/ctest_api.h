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
#define ASSERT_TRUE(condition) \
	assertTrue(__FILE__, __LINE__, condition)

void assertTrue(const char *filePath, int line, bool condition);

/**
 * Tests that the given `condition` is `true`
 */
#define ASSERT_FALSE(condition) \
	assertFalse(__FILE__, __LINE__, condition)

void assertFalse(const char *filePath, int line, bool condition);

/**
 * Tests that the given pointer is `NULL`
 */
#define ASSERT_NULL(pointer) \
	assertFalse(__FILE__, __LINE__, pointer)

void assertNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the given pointer is not `NULL`
 */
#define ASSERT_NON_NULL(pointer) \
	assertNonNull(__FILE__, __LINE__, pointer)

void assertNonNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the given 8 bit integers have the same value
 */
#define ASSERT_8_BIT_INT_EQUALS(expected, actual) \
	assert8BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert8BitIntEquals(const char *filePath, int line, uint8_t expected, uint8_t actual);

/**
 * Tests that the given 16 bit integers have the same value
 */
#define ASSERT_16_BIT_INT_EQUALS(expected, actual) \
	assert16BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert16BitIntEquals(const char *filePath, int line, uint16_t expected, uint16_t actual);

/**
 * Tests that the given longs have the same value
 */
#define ASSERT_LONG_EQUALS(expected, actual) \
	assertLongEquals(__FILE__, __LINE__, expected, actual)

void assertLongEquals(const char *filePath, int line, long expected, long actual);

/**
 * Tests that the given strings are similar
 */
#define ASSERT_STRING_EQUALS(expected, actual) \
	assertStringEquals(__FILE__, __LINE__, expected, actual)

void assertStringEquals(const char *filePath, int line, const char *expected, const char *actual);

#endif // __CTEST_API_H
