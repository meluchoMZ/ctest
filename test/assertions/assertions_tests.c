/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include <ctest_api.h>

TEST(TestBasic, test_assert_true, Tests that the `assertTrue` function works properly)
{
	assertTrue(1==1);
}

TEST(TestBasic, test_several_asserts, Test that several asserts can be chained)
{
	printf("before assert 1\n");
	assertTrue(1==1);
	printf("before assert 2\n");
	assertTrue(1==1);
	printf("after assertions\n");
}

TEST(TestBasic, test_failure_ends_test, Test that a failure ends a test-case execution)
{
	//assertTrue(1==2);
	printf("should have failed and this should not be visible\n");
}

TEST(TestBasic, test_after_previous_test_failure, Tests that a failure does not interrupt other test execution)
{
	printf("should be visible executed after `test_failure_ends_test`\n");
}
