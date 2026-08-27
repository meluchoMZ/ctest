/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"
#include "../../src/ctest_core.h"

#include <stdlib.h>

void sampleTestFunction(void)
{
	return;
}

void assertPointerEquals(TestFunction expected, TestFunction actual)
{
	if (!(expected == actual)) {
		_Exit(EXIT_FAILURE);
	}
}

TEST(TestCaseTests, createTestCase, Tests that a TestCase is created correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	ASSERT_NON_NULL(testCase);
	ASSERT_STRING_EQUALS(name, testCase->name);
	ASSERT_STRING_EQUALS(suite, testCase->suiteName);
	ASSERT_STRING_EQUALS(desc, testCase->description);
	assertPointerEquals(sampleTestFunction, testCase->execute);
	free(testCase);
}

TEST(TestCaseTests, freeTestCase, Tests that a TestCase is freed correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	ASSERT_NON_NULL(testCase);
	ASSERT_STRING_EQUALS(name, testCase->name);
	ASSERT_STRING_EQUALS(suite, testCase->suiteName);
	ASSERT_STRING_EQUALS(desc, testCase->description);
	assertPointerEquals(sampleTestFunction, testCase->execute);
	freeTestCase(testCase);
}

TEST(TestCaseTests, addTestCaseToSuite, Tests that a TestCase is correctly added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	ASSERT_NON_NULL(testCase);
	TestSuite *testSuite = createTestSuite(suite);
	ASSERT_NON_NULL(testSuite);
	ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseAlreadyAdded, Tests that a already added TestCase is not added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	ASSERT_NON_NULL(testCase);
	TestSuite *testSuite = createTestSuite(suite);
	ASSERT_NON_NULL(testSuite);
	ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(1, testSuite->testCount);
	addTestCase(testSuite, testCase);
	ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseToSuiteForcingRealloc, Tests that a TestCase is correctly added to a TestSuite when a realloc is forced)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	ASSERT_NON_NULL(testCase);
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	ASSERT_NON_NULL(testSuite);
	testSuite->testCases = malloc(sizeof(TestCase));
	ASSERT_NON_NULL(testSuite->testCases);
	testSuite->testCaseSize = 1;
	testSuite->testCount = 0;
	ASSERT_LONG_EQUALS(1, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	ASSERT_LONG_EQUALS(1, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(1, testSuite->testCount);
	const char *name2 = "testCase2";
	TestCase *testCase2 = createTestCase(name2, suite, desc, sampleTestFunction);
	addTestCase(testSuite, testCase2);
	ASSERT_LONG_EQUALS(11, testSuite->testCaseSize);
	ASSERT_LONG_EQUALS(2,  testSuite->testCount);
	freeTestSuite(&testSuite);
}
