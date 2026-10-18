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

void assertFunctionPointerEquals(CTest_TestFunction expected, CTest_TestFunction actual)
{
	if (!(expected == actual)) {
		_Exit(EXIT_FAILURE);
	}
}

TEST(TestCaseTests, createTestCase, Tests that a CTest_TestCase is created correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	CTest_TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	CT_ASSERT_NON_NULL(testCase);
	CT_ASSERT_STRING_EQUALS(name, testCase->name);
	CT_ASSERT_STRING_EQUALS(suite, testCase->suiteName);
	CT_ASSERT_STRING_EQUALS(desc, testCase->description);
	assertFunctionPointerEquals(sampleTestFunction, testCase->execute);
	free(testCase);
}

TEST(TestCaseTests, freeTestCase, Tests that a CTest_TestCase is freed correctly)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	CTest_TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	CT_ASSERT_NON_NULL(testCase);
	CT_ASSERT_STRING_EQUALS(name, testCase->name);
	CT_ASSERT_STRING_EQUALS(suite, testCase->suiteName);
	CT_ASSERT_STRING_EQUALS(desc, testCase->description);
	assertFunctionPointerEquals(sampleTestFunction, testCase->execute);
	freeTestCase(testCase);
}

TEST(TestCaseTests, addTestCaseToSuite, Tests that a CTest_TestCase is correctly added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	CTest_TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	CT_ASSERT_NON_NULL(testCase);
	CTest_TestSuite *testSuite = createTestSuite(suite);
	CT_ASSERT_NON_NULL(testSuite);
	CT_ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	CT_ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseAlreadyAdded, Tests that a already added CTest_TestCase is not added to a TestSuite)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	CTest_TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	CT_ASSERT_NON_NULL(testCase);
	CTest_TestSuite *testSuite = createTestSuite(suite);
	CT_ASSERT_NON_NULL(testSuite);
	CT_ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	CT_ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCount);
	addTestCase(testSuite, testCase);
	CT_ASSERT_LONG_EQUALS(10, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCount);
	freeTestSuite(&testSuite);
}

TEST(TestCaseTests, addTestCaseToSuiteForcingRealloc, Tests that a CTest_TestCase is correctly added to a CTest_TestSuite when a realloc is forced)
{
	const char *name = "testCase";
	const char *suite = "sutie";
	const char *desc = "description";
	CTest_TestCase *testCase = createTestCase(name, suite, desc, sampleTestFunction);
	CT_ASSERT_NON_NULL(testCase);
	CTest_TestSuite *testSuite = malloc(sizeof(CTest_TestSuite));
	CT_ASSERT_NON_NULL(testSuite);
	testSuite->testCases = malloc(sizeof(CTest_TestCase));
	CT_ASSERT_NON_NULL(testSuite->testCases);
	testSuite->testCaseSize = 1;
	testSuite->testCount = 0;
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(0, testSuite->testCount);
	addTestCase(testSuite, testCase);
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(1, testSuite->testCount);
	const char *name2 = "testCase2";
	CTest_TestCase *testCase2 = createTestCase(name2, suite, desc, sampleTestFunction);
	addTestCase(testSuite, testCase2);
	CT_ASSERT_LONG_EQUALS(11, testSuite->testCaseSize);
	CT_ASSERT_LONG_EQUALS(2,  testSuite->testCount);
	freeTestSuite(&testSuite);
}
