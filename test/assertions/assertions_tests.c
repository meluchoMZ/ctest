/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include <ctest_api.h>

TEST(TestBasic, test_assert_true, Tests that the `assertTrue` function works properly)
{
	ASSERT_TRUE(1==1);
}

TEST(TestBasic, test_assert_false, Tests that the `ASSERT_FALSE` function works properly)
{
	ASSERT_FALSE(1==2);
}

