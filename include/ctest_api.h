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
				registerTestInSuite(testSuite, #name, #description, NULL, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, NULL, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)

#define TEST_EXPECT_FAIL(suite, name, description, expectedError) \
	static void __attribute__((unused)) suite##_##name##_impl(void); \
	static void __attribute__((constructor)) suite##_##name##_register(void) \
		{ \
		TestSuite *testSuite = findSuite(testStatus, #suite); \
		if (testSuite == NULL) { \
			TestSuite *testSuite = createTestSuite(#suite); \
			if (!addTestSuite(testStatus, testSuite)) { \
				freeTestSuite(&testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, expectedError, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, expectedError, suite##_##name##_impl); \
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
	assertNull(__FILE__, __LINE__, pointer)

void assertNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the given pointer is not `NULL`
 */
#define ASSERT_NON_NULL(pointer) \
	assertNonNull(__FILE__, __LINE__, pointer)

void assertNonNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	assert8BitUIntEquals(__FILE__, __LINE__, expected, actual)

void assert8BitUIntEquals(const char *filePath, int line, uint8_t expected, uint8_t actual);

/**
 * Tests that the given 16 bit unsigned integers have the same value
 */
#define ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	assert16BitUIntEquals(__FILE__, __LINE__, expected, actual)

void assert16BitUIntEquals(const char *filePath, int line, uint16_t expected, uint16_t actual);

/**
 * Tests that the given 32 bit unsigned integers have the same value
 */
#define ASSERT_32_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	assert32BitUIntEquals(__FILE__, __LINE__, expected, actual)

void assert32BitUIntEquals(const char *filePath, int line, uint32_t expected, uint32_t actual);

/**
 * Tests that the given 64 bit unsigned integers have the same value
 */
#define ASSERT_64_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	assert64BitUIntEquals(__FILE__, __LINE__, expected, actual)

void assert64BitUIntEquals(const char *filePath, int line, uint64_t expected, uint64_t actual);

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define ASSERT_8_BIT_SIGNED_INT_EQUALS(expected, actual) \
	assert8BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert8BitIntEquals(const char *filePath, int line, int8_t expected, int8_t actual);

/**
 * Tests that the given 16 bit signed integers have the same value
 */
#define ASSERT_16_BIT_SIGNED_INT_EQUALS(expected, actual) \
	assert16BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert16BitIntEquals(const char *filePath, int line, int16_t expected, int16_t actual);

/**
 * Tests that the given 32 bit signed integers have the same value
 */
#define ASSERT_32_BIT_SIGNED_INT_EQUALS(expected, actual) \
	assert32BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert32BitIntEquals(const char *filePath, int line, int32_t expected, int32_t actual);

/**
 * Tests that the given 64 bit signed integers have the same value
 */
#define ASSERT_64_BIT_SIGNED_INT_EQUALS(expected, actual) \
	assert64BitIntEquals(__FILE__, __LINE__, expected, actual)

void assert64BitIntEquals(const char *filePath, int line, int64_t expected, int64_t actual);


/**
 * Tests that the given integers have the same value
 */
#define ASSERT_INT_EQUALS(expected, actual) \
	assertIntEquals(__FILE__, __LINE__, expected, actual)

void assertIntEquals(const char *filePath, int line, long expected, long actual);

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
