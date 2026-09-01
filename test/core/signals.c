/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"

#include <signal.h>

TEST_EXPECT_FAIL(TestSignalHandlers, sigabrt, Tests that abort signals are correctly caught,
		"Received Aborted (SIGABRT)")
{
	raise(SIGABRT);
}

TEST_EXPECT_FAIL(TestSignalHandlers, sigalrm, Tests timeouts do not end tests,
		"Received timeout (SIGALRM)")
{
	raise(SIGALRM);
}

TEST_EXPECT_FAIL(TestSignalHandlers, sigbus, Test memory alignment/physical page execeptions,
		"Received Bus error (SIGBUS)")
{
	raise(SIGBUS);
}

TEST(TestSignalHandlers, sigchld, Tests that a sigchild signal does not end the execution)
{
	raise(SIGCHLD);
}
TEST_EXPECT_FAIL(TestSignalHandlers, sigint, Tests that a sigint faults are correctly caught,
		"Received Interrupt (SIGINT)")
{
	raise(SIGINT);
}

TEST_EXPECT_FAIL(TestSignalHandlers, sigsegv, Tests that segmentation faults are correctly caught,
		"Received Segmentation fault (SIGSEGV)")
{
	raise(SIGSEGV);
}
