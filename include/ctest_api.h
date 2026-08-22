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
				freeTestSuite(testSuite); \
			} else { \
				registerTestInSuite(testSuite, #name, #description, suite##_##name##_impl); \
			} \
		} else { \
			registerTestInSuite(testSuite, #name, #description, suite##_##name##_impl); \
		} \
	} \
	static void __attribute__((unused)) suite##_##name##_impl(void)

// Public documented test functions
void assertTrue(bool condition);
void assertFalse(bool condition);
void assertNull(void *pointer);
void assertNonNull(void *pointer);
void assertByteEquals(uint8_t byte1, uint8_t byte2);
void assertShortEquals(uint16_t short1, uint16_t short2);

#endif // __CTEST_API_H
