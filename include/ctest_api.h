/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#ifndef __CTEST_API_H
#define __CTEST_API_H

#include "ctest_core.h"

#include <stdbool.h>
#include <stdint.h>

// Macros for test declarations
#define TEST(suite, name, description) \
	static void __attribute__((unused)) suite##_##name##_impl(void); \
	static void __attribute__((constructor)) suite##_##name##_register(void) \
		{ \
		CTest_TestSuite *testSuite = findSuite(testStatus, #suite); \
		if (testSuite == NULL) { \
			CTest_TestSuite *testSuite = createTestSuite(#suite); \
			if (!addTestSuite(testStatus, testSuite)) { \
				freeTestSuite(&testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, NULL, false, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, NULL, false, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)

#define TEST_EXPECT_FAIL(suite, name, description, expectedError) \
	static void __attribute__((unused)) suite##_##name##_impl(void); \
	static void __attribute__((constructor)) suite##_##name##_register(void) \
		{ \
		CTest_TestSuite *testSuite = findSuite(testStatus, #suite); \
		if (testSuite == NULL) { \
			CTest_TestSuite *testSuite = createTestSuite(#suite); \
			if (!addTestSuite(testStatus, testSuite)) { \
				freeTestSuite(&testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, expectedError, false, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, expectedError, false, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)

#define TEST_EXPECT_FAIL_REGEXP(suite, name, description, expectedError) \
	static void __attribute__((unused)) suite##_##name##_impl(void); \
	static void __attribute__((constructor)) suite##_##name##_register(void) \
		{ \
		CTest_TestSuite *testSuite = findSuite(testStatus, #suite); \
		if (testSuite == NULL) { \
			CTest_TestSuite *testSuite = createTestSuite(#suite); \
			if (!addTestSuite(testStatus, testSuite)) { \
				freeTestSuite(&testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, expectedError, true, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, expectedError, true, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)


// Public documented test functions

/**
 * Tests that the given `condition` is `true`
 */
#define CT_ASSERT_TRUE(condition) \
	CT_AssertTrue(__FILE__, __LINE__, condition)

void CT_AssertTrue(const char *filePath, int line, bool condition);

/**
 * Tests that the given `condition` is `true`
 */
#define CT_ASSERT_FALSE(condition) \
	CT_AssertFalse(__FILE__, __LINE__, condition)

void CT_AssertFalse(const char *filePath, int line, bool condition);

/**
 * Tests that the given pointer is `NULL`
 */
#define CT_ASSERT_NULL(pointer) \
	CT_AssertNull(__FILE__, __LINE__, pointer)

void CT_AssertNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the given pointer is not `NULL`
 */
#define CT_ASSERT_NON_NULL(pointer) \
	CT_AssertNonNull(__FILE__, __LINE__, pointer)

void CT_AssertNonNull(const char *filePath, int line, void *pointer);

/**
 * Tests that the two pointers are equal.
 * It only compares raw pointer values, no type is taken into account
 */
#define CT_ASSERT_POINTER_EQUALS(expected, actual) \
	CT_AssertPointerEquals(__FILE__, __LINE__, expected, actual)

void CT_AssertPointerEquals(const char *filePath, int line, void *expected, void *actual);

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define CT_ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert8BitUIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert8BitUIntEquals(const char *filePath, int line, uint8_t expected, uint8_t actual);

/**
 * Tests that the given 16 bit unsigned integers have the same value
 */
#define CT_ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert16BitUIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert16BitUIntEquals(const char *filePath, int line, uint16_t expected, uint16_t actual);

/**
 * Tests that the given 32 bit unsigned integers have the same value
 */
#define CT_ASSERT_32_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert32BitUIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert32BitUIntEquals(const char *filePath, int line, uint32_t expected, uint32_t actual);

/**
 * Tests that the given 64 bit unsigned integers have the same value
 */
#define CT_ASSERT_64_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert64BitUIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert64BitUIntEquals(const char *filePath, int line, uint64_t expected, uint64_t actual);

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define CT_ASSERT_8_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert8BitIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert8BitIntEquals(const char *filePath, int line, int8_t expected, int8_t actual);

/**
 * Tests that the given 16 bit signed integers have the same value
 */
#define CT_ASSERT_16_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert16BitIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert16BitIntEquals(const char *filePath, int line, int16_t expected, int16_t actual);

/**
 * Tests that the given 32 bit signed integers have the same value
 */
#define CT_ASSERT_32_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert32BitIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert32BitIntEquals(const char *filePath, int line, int32_t expected, int32_t actual);

/**
 * Tests that the given 64 bit signed integers have the same value
 */
#define CT_ASSERT_64_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert64BitIntEquals(__FILE__, __LINE__, expected, actual)

void CT_Assert64BitIntEquals(const char *filePath, int line, int64_t expected, int64_t actual);


/**
 * Tests that the given integers have the same value
 */
#define CT_ASSERT_INT_EQUALS(expected, actual) \
	CT_AssertIntEquals(__FILE__, __LINE__, expected, actual)

void CT_AssertIntEquals(const char *filePath, int line, long expected, long actual);

/**
 * Tests that the given longs have the same value
 */
#define CT_ASSERT_LONG_EQUALS(expected, actual) \
	CT_AssertLongEquals(__FILE__, __LINE__, expected, actual)

void CT_AssertLongEquals(const char *filePath, int line, long expected, long actual);

/**
 * Tests that the given strings are similar
 */
#define CT_ASSERT_STRING_EQUALS(expected, actual) \
	CT_AssertStringEquals(__FILE__, __LINE__, expected, actual)

void CT_AssertStringEquals(const char *filePath, int line, const char *expected, const char *actual);

#endif // __CTEST_API_H
