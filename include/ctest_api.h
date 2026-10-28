#ifndef DOXYGEN_SHOULD_SKIP_THIS
/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/**
 * @mainpage CTest Library Documentation
 * @section intro_sec Introduction
 * **CTest** is a lightweight, zero-boilerplate C testing library. 
 *
 * It is intended to be an alternative for C testing avoiding the following issues present on popular frameworks:
 * - **Boilerplate**: Most C testing frameworks involve a lot of setup code needed to simply execute a test.
 * - **C++ toolchain and compilation speed**: Many frameworks rely on other toolchans (other than C) that increment compilation times singificantly.
 *
 * @section features_sec Features
 * - **Auto-Registration**: Write tests without manually adding them to a @c main() function or maintaining test suites.
 * - **Minimal Boilerplate**: Define tests using intuitive macros and let CTest handle execution and setup.
 * - **Test functions**: Support for general test assertions and equality functions.
 * - **Test failures with regexp support**: Expect tests to fail, check error messages with full regexp support.
 * - **Test failure file/line signaling**: The library points to the source file and line that failed in the test execution, with full stack trace.
 *
 * @section installation_sec Installation
 *
 * @subsection deps Dependencies
 * CTest does only rely on core @c libc dependencies, so it should compile out of the box on most environments.
 *
 * @subsection build_from_source Build from source
 * Clone the repository and install CTest globally using @c make:
 * @code{.sh}
 * git clone https://github.com/meluchoMZ/ctest.git
 * cd ctest
 * make
 * sudo make install
 * @endcode
 *
 * @section quickstart_sec Quick Start Guide
 * Include `<ctest_api.h>` in your test source files, write your test cases, and compile. No explicit @c main() wrapper or test setup code is required.
 *
 * @code{.c}
 * #include <ctest_api.h>
 *
 * TEST(MathSuite, AdditionTest, "Verifies basic addition operations") {
 *     int result = 2 + 2;
 *     CT_ASSERT_TRUE(result == 4);
 * }
 *
 * TEST(MathSuite, SubtractionTest, "Verifies basic subtraction operations") {
 *     int result = 5 - 3;
 *     CT_ASSERT_INT_EQUALS(0, result);
 * }
 * @endcode
 *
 * Then, compile your test with:
 * @code{.sh}
 * gcc -o test_example test_example.c -lctest -rdynamic -g
 * ./test_example
 * @endcode
 * - `-lctest` to link CTest.
 * - `-rdynamic` and `-g` for stack trace printing.
 *
 * @section usage_sec Usage
 * Executing the compiled executable will run all compiled tests.
 * - You can filter for a test suite using the `-s` (`--suite`) option.
 * - You can filter by a test case using the `-c` (`--case`) option.
 *
 * @section api_sec API Reference
 * Explore the API provided by CTest:
 * - @ref ctest_test_def
 * - @ref ctest_assert_def
 *
 * @section internal_docu Development (non public, internal) documentation
 * Explore the internal development (non public) documentation for the library:
 * - @ref ctest_internal
 */

#ifndef __CTEST_API_H
#define __CTEST_API_H

#include "ctest_core.h"

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup ctest_test_def CTest test definition macros
 *
 * @brief Macros to define test cases.
 *
 * @{
 */

/**
 * @def TEST(suite, name, description)
 * @brief Defines a standard test case and adds it to a test suite.
 * @param suite The name of the test suite.
 * @param name The name of the specific test.
 * @param description A short description of what the test does.
 */
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

/**
 * @def TEST_EXPECT_FAIL(suite, name, description, expectedError)
 * @brief Defines a test case that expects to fail. Will catch the failure and search for the given expectedError in the logs.
 * @param suite The name of the test suite.
 * @param name The name of the specific test.
 * @param description A short description of what the test does.
 * @param expectedError The literal string that the test will match when the fail occurs.
 */
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

/**
 * @def TEST_EXPECT_FAIL_REGEXP(suite, name, description, expectedError)
 * @brief Defines a test case that expects to fail. Will catch the failure and search for the given expectedError in the logs. Same as #TEST_EXPECT_FAIL(suite, name, description, expectedError) but the expectedError can be a regular expression.
 * @param suite The name of the test suite.
 * @param name The name of the specific test.
 * @param description A short description of what the test does.
 * @param expectedError A regular expression that the test will match when the fail occurs.
 */
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

/** @} */

// Public documented test macros

/**
 * @defgroup ctest_assert_def CTest assertion definition macros
 *
 * @brief Macros to execute assertions in tests.
 *
 * @{
 */
/**
 * Tests that the given `condition` is `true`
 */
#define CT_ASSERT_TRUE(condition) \
	CT_AssertTrue(__FILE__, __LINE__, condition)

/**
 * Tests that the given `condition` is `true`
 */
#define CT_ASSERT_FALSE(condition) \
	CT_AssertFalse(__FILE__, __LINE__, condition)

/**
 * Tests that the given pointer is `NULL`
 */
#define CT_ASSERT_NULL(pointer) \
	CT_AssertNull(__FILE__, __LINE__, pointer)

/**
 * Tests that the given pointer is not `NULL`
 */
#define CT_ASSERT_NON_NULL(pointer) \
	CT_AssertNonNull(__FILE__, __LINE__, pointer)

/**
 * Tests that the two pointers are equal.
 * It only compares raw pointer values, no type is taken into account
 */
#define CT_ASSERT_POINTER_EQUALS(expected, actual) \
	CT_AssertPointerEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define CT_ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert8BitUIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 16 bit unsigned integers have the same value
 */
#define CT_ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert16BitUIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 32 bit unsigned integers have the same value
 */
#define CT_ASSERT_32_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert32BitUIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 64 bit unsigned integers have the same value
 */
#define CT_ASSERT_64_BIT_UNSIGNED_INT_EQUALS(expected, actual) \
	CT_Assert64BitUIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 8 bit unsigned integers have the same value
 */
#define CT_ASSERT_8_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert8BitIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 16 bit signed integers have the same value
 */
#define CT_ASSERT_16_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert16BitIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 32 bit signed integers have the same value
 */
#define CT_ASSERT_32_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert32BitIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given 64 bit signed integers have the same value
 */
#define CT_ASSERT_64_BIT_SIGNED_INT_EQUALS(expected, actual) \
	CT_Assert64BitIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given integers have the same value
 */
#define CT_ASSERT_INT_EQUALS(expected, actual) \
	CT_AssertIntEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given longs have the same value
 */
#define CT_ASSERT_LONG_EQUALS(expected, actual) \
	CT_AssertLongEquals(__FILE__, __LINE__, expected, actual)

/**
 * Tests that the given strings are similar
 */
#define CT_ASSERT_STRING_EQUALS(expected, actual) \
	CT_AssertStringEquals(__FILE__, __LINE__, expected, actual)

/** @} */

void CT_AssertTrue(const char *filePath, int line, bool condition);
void CT_AssertFalse(const char *filePath, int line, bool condition);
void CT_AssertNull(const char *filePath, int line, void *pointer);
void CT_AssertNonNull(const char *filePath, int line, void *pointer);
void CT_AssertPointerEquals(const char *filePath, int line, void *expected, void *actual);
void CT_Assert8BitUIntEquals(const char *filePath, int line, uint8_t expected, uint8_t actual);
void CT_Assert16BitUIntEquals(const char *filePath, int line, uint16_t expected, uint16_t actual);
void CT_Assert32BitUIntEquals(const char *filePath, int line, uint32_t expected, uint32_t actual);
void CT_Assert64BitUIntEquals(const char *filePath, int line, uint64_t expected, uint64_t actual);
void CT_Assert8BitIntEquals(const char *filePath, int line, int8_t expected, int8_t actual);
void CT_Assert16BitIntEquals(const char *filePath, int line, int16_t expected, int16_t actual);
void CT_Assert32BitIntEquals(const char *filePath, int line, int32_t expected, int32_t actual);
void CT_Assert64BitIntEquals(const char *filePath, int line, int64_t expected, int64_t actual);
void CT_AssertIntEquals(const char *filePath, int line, long expected, long actual);
void CT_AssertLongEquals(const char *filePath, int line, long expected, long actual);
void CT_AssertStringEquals(const char *filePath, int line, const char *expected, const char *actual);

#endif // __CTEST_API_H
