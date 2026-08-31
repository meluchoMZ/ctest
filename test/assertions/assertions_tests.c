/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include <ctest_api.h>
#include <stdlib.h>

TEST(Assertions, assertTrue, Tests that the `ASSERT_TRUE` function works properly)
{
	ASSERT_TRUE(1==1);
}

TEST(Assertions, assertFalse, Tests that the `ASSERT_FALSE` function works properly)
{
	ASSERT_FALSE(1==2);
}

TEST(Assertions, assertNull, Tests that the `ASSERT_NULL` function works properly)
{
	ASSERT_NULL(NULL);
}

TEST(Assertions, assertNonNull, Tests that the `ASSERT_NON_NULL` function works properly)
{
	int x = 0;
	ASSERT_NON_NULL(&x);
}

TEST(Assertions, assert8BitUnsignedEquals, Tests equals on 8 bit unsigned integers)
{
	uint8_t expected = 8;
	uint8_t actual = 8;
	ASSERT_8_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert16BitUnsignedEquals, Tests equals on 16 bit unsigned integers)
{
	uint16_t expected = 16;
	uint16_t actual = 16;
	ASSERT_16_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert32BitUnsignedEquals, Tests equals on 32 bit unsigned integers)
{
	uint32_t expected = 16;
	uint32_t actual = 16;
	ASSERT_32_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert64BitUnsignedEquals, Tests equals on 64 bit unsigned integers)
{
	uint64_t expected = 16;
	uint64_t actual = 16;
	ASSERT_64_BIT_UNSIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert8BitSignedEquals, Tests equals on 8 bit signed integers)
{
	int8_t expected = 8;
	int8_t actual = 8;
	ASSERT_8_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert16BitSignedEquals, Tests equals on 16 bit signed integers)
{
	int16_t expected = 16;
	int16_t actual = 16;
	ASSERT_16_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert32BitSignedEquals, Tests equals on 32 bit signed integers)
{
	int32_t expected = 16;
	int32_t actual = 16;
	ASSERT_32_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assert64BitSignedEquals, Tests equals on 64 bit signed integers)
{
	int64_t expected = 16;
	int64_t actual = 16;
	ASSERT_64_BIT_SIGNED_INT_EQUALS(expected, actual);
}

TEST(Assertions, assertIntEquals, Tests equals on integers)
{
	int expected = 392857;
	int actual = 392857;
	ASSERT_INT_EQUALS(expected, actual);
}

TEST(Assertions, assertLongEquals, Tests equals on long integers)
{
	long expected = 392857;
	long actual = 392857;
	ASSERT_LONG_EQUALS(expected, actual);
}

TEST(Assertions, assertStringEquals, Tests equals on char strings)
{
	const char *expected = "asdfkj";
	const char *actual = "asdfkj";
	ASSERT_STRING_EQUALS(expected, actual);
}
