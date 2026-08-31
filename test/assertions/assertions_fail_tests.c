/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include <ctest_api.h>
#include <stdlib.h>

TEST_EXPECT_FAIL(AssertionsShouldFail, assertTrueFails, "Tests that the `ASSERT_TRUE` function works properly", 
		"assertTrue | actual: FALSE, expected: TRUE")
{
	ASSERT_TRUE(1==2);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertFalseFails, Tests that the `ASSERT_FALSE` function works properly, 
		"assertFalse | actual: TRUE, expected: FALSE")
{
	ASSERT_FALSE(1==1);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertNullFails, Tests that the `ASSERT_NULL` function works properly, 
		"assertNull | actual: 0x7ffcae848664, expected: NULL")
{
	int x = 1231;
	int *ptr = &x;
	ASSERT_NULL(ptr);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertNonNullFails, Tests that the `ASSERT_NON_NULL` function works properly, 
		"assertNonNull | actual: NULL, expected: (nil)")
{
	ASSERT_NON_NULL(NULL);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert8BitUnsignedEqualsFails, Tests equals on 8 bit unsigned integers, 
		"assert8BitUIntEquals | actual: 9, expected: 8")
{
	uint8_t expected = 8;
	uint8_t actual = 9;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert16BitUnsignedEqualsFails, Tests equals on 16 bit unsigned integers, 
		"assert16BitUIntEquals | actual: 17, expected: 16")
{
	uint16_t expected = 16;
	uint16_t actual = 17;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert32BitUnsignedEqualsFails, Tests equals on 32 bit unsigned integers, 
		"assert32BitUIntEquals | actual: 17, expected: 16")
{
	uint32_t expected = 16;
	uint32_t actual = 17;
	ASSERT_32_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert64BitUnsignedEqualsFails, Tests equals on 64 bit unsigned integers, 
		"assert64BitUIntEquals | actual: 17, expected: 16")
{
	uint64_t expected = 16;
	uint64_t actual = 17;
	ASSERT_64_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert8BitSignedEqualsFails, Tests equals on 8 bit signed integers, 
		"assert8BitIntEquals | actual: 9, expected: 8")
{
	int8_t expected = 8;
	int8_t actual = 9;
	ASSERT_8_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert16BitSignedEqualsFails, Tests equals on 16 bit signed integers, 
		"assert16BitIntEquals | actual: 17, expected: 16")
{
	int16_t expected = 16;
	int16_t actual = 17;
	ASSERT_16_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert32BitSignedEqualsFails, Tests equals on 32 bit signed integers, 
		"assert32BitIntEquals | actual: 17, expected: 16")
{
	int32_t expected = 16;
	int32_t actual = 17;
	ASSERT_32_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assert64BitSignedEqualsFails, Tests equals on 64 bit signed integers, 
		"assert64BitIntEquals | actual: 17, expected: 16")
{
	int64_t expected = 16;
	int64_t actual = 17;
	ASSERT_64_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertIntEqualsFails, Tests equals on integers, 
		"assertIntEquals | actual: 392858, expected: 392857")
{
	int expected = 392857;
	int actual = 392858;
	ASSERT_INT_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertLongEqualsFails, Tests equals on long integers, 
		"assertLongEquals | actual: 392877, expected: 392857")
{
	long expected = 392857;
	long actual = 392877;
	ASSERT_LONG_EQUALS(expected, actual);
}

TEST_EXPECT_FAIL(AssertionsShouldFail, assertStringEqualsFail, Tests equals on char strings, 
		"assertStringEquals | actual: asdkkj, expected: asdfkj")
{
	const char *expected = "asdfkj";
	const char *actual = "asdkkj";
	ASSERT_STRING_EQUALS(expected, actual);
}
