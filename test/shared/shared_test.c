/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

/*
 * This test file is intended to test 
 * the shared library installion by 
 * compiling and executing tests against
 * the shared lib (-lctest)
 */

#include <ctest_api.h>

TEST(ClientTests, testSimpleAssertion, "Tests client usage by executing tests against the shared library, without direct static compilation")
{
	CT_ASSERT_TRUE(1 == 1);
}
