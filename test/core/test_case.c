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
		exit(EXIT_FAILURE);
	}
}

TEST(TestCaseTests, createTestCase, Tests that a TestCase is created correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	assertNonNull(testCase);
	assertStringEquals(name, testCase->name);
	assertStringEquals(suite, testCase->suiteName);
	assertStringEquals(desc, testCase->description);
	assertPointerEquals(sampleTestFunction, testCase->execute);
	free(testCase);
}

TEST(TestCaseTests, freeTestCase, Tests that a TestCase is freed correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	assertNonNull(testCase);
	assertStringEquals(name, testCase->name);
	assertStringEquals(suite, testCase->suiteName);
	assertStringEquals(desc, testCase->description);
	assertPointerEquals(sampleTestFunction, testCase->execute);
	// if the function does not frees the memory, the test will fail due to the address sanitizer
	freeTestCase(testCase);
}

TEST(TestCaseTests, addTestCaseToSuite, Tests that a TestCase is correctly added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	assertNonNull(testCase);
	TestSuite *testSuite = createTestSuite(suite);
	assertNonNull(testSuite);
	assertLongEquals(10, testSuite->testCaseSize);
	assertLongEquals(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	assertLongEquals(10, testSuite->testCaseSize);
	assertLongEquals(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseAlreadyAdded, Tests that a already added TestCase is not added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	assertNonNull(testCase);
	TestSuite *testSuite = createTestSuite(suite);
	assertNonNull(testSuite);
	assertLongEquals(10, testSuite->testCaseSize);
	assertLongEquals(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	assertLongEquals(10, testSuite->testCaseSize);
	assertLongEquals(1, testSuite->testCount);
	addTestCase(testSuite, testCase);
	assertLongEquals(10, testSuite->testCaseSize);
	assertLongEquals(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseToSuiteForcingRealloc, Tests that a TestCase is correctly added to a TestSuite when a realloc is forced)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	assertNonNull(testCase);
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	assertNonNull(testSuite);
	testSuite->testCases = malloc(sizeof(TestCase));
	assertNonNull(testSuite->testCases);
	testSuite->testCaseSize = 1;
	testSuite->testCount = 0;
	assertLongEquals(1, testSuite->testCaseSize);
	assertLongEquals(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	assertLongEquals(1, testSuite->testCaseSize);
	assertLongEquals(1, testSuite->testCount);
	const char *name2 = "testCase2";
	TestCase *testCase2 = createTestCase(name2, suite, desc, sampleTestFunction);
	addTestCase(testSuite, testCase2);
	//assertLongEquals(12, testSuite->testCaseSize);
	assertLongEquals(2,  testSuite->testCount);
	freeTestSuite(&testSuite);
}
