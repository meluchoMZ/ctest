/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"

#include <signal.h>

TEST_EXPECT_FAIL(TestSignalHandlers, sigsegvTest, Tests that segmentation faults are correctly caught,
		"Received Segmentation fault (SIGSEGV)")
{
	raise(SIGSEGV);
}

TEST_EXPECT_FAIL(TestSignalHandlers, sigintTest, Tests that a sigint faults are correctly caught,
		"Received Interrupt (SIGINT)")
{
	raise(SIGINT);
}
