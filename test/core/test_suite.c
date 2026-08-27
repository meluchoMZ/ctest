/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"
#include "../../src/ctest_core.h"

TEST(TestSuitesTests, createTestSuite, Tests the creation of a TestSuite element)
{
	const char * expectedSuiteName = "testSuite";
	TestSuite *actual = createTestSuite(expectedSuiteName);
	ASSERT_STRING_EQUALS(expectedSuiteName, actual->name);
	ASSERT_NON_NULL(actual->testCases);
	ASSERT_LONG_EQUALS(10, actual->testCaseSize);
	ASSERT_LONG_EQUALS(0, actual->testCount);
	free(actual->testCases);
	actual->testCases = NULL;
	free(actual);
	actual = NULL;
}

TEST(TestSuitesTests, freeTestSuite, Tests the liberation of a TestSuite element)
{
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	ASSERT_NON_NULL(testSuite);
	testSuite->testCases = malloc(sizeof(TestCase));
	ASSERT_NON_NULL(testSuite->testCases);
	freeTestSuite(&testSuite);
}

TEST(TestSuitesTests, freeTestSuiteWithNullTestCases, Tests the liberation of a TestSuite element if the inner TestCase pointer is NULL)
{
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	ASSERT_NON_NULL(testSuite);
	testSuite->testCases = NULL;
	ASSERT_NULL(testSuite->testCases);
	freeTestSuite(&testSuite);
	ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, freeNullTestSuite, Tests the liberation of a TestSuite element does not double free a NULL TestSuite)
{
	TestSuite *testSuite = NULL;
	ASSERT_NULL(testSuite);
	freeTestSuite(&testSuite);
	ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, addTestSuite, Tests the addition of a TestSuite)
{
	const char *testName = "test";
	TestStatus testStatus;
	testStatus.testSuites = malloc(sizeof(TestSuite));
	ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	TestSuite *testSuite = createTestSuite(testName);
	ASSERT_NON_NULL(testSuite);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 1);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	ASSERT_NULL(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, addTestSuiteDuplicate, Tests the addition of a TestSuite twice)
{
	const char *testName = "test";
	TestStatus testStatus;
	testStatus.testSuites = malloc(2* sizeof(TestSuite));
	ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 2;
	testStatus.suiteCount = 0;
	TestSuite *testSuite = createTestSuite(testName);
	ASSERT_NON_NULL(testSuite);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	ASSERT_NULL(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, findTestSuiteNullTestStatus, Tests the findSuite does not crash when teh testStatus is NULL)
{
	const char *suiteName = "test";
	TestSuite *testSuite = findSuite(NULL, suiteName);
	ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, findTestSuite, Tests the TestSuite element search function by name over the TestStatus element)
{
	const char *testName = "test";
	const char *example1Name = "example1";
	const char *example2Name = "example2";
	const char *example3Name = "example3";
	TestSuite *expected = createTestSuite(testName);
	TestSuite *example1 = createTestSuite(example1Name);
	TestSuite *example2 = createTestSuite(example2Name);
	TestSuite *example3 = createTestSuite(example3Name);
	TestStatus testStatus;
	testStatus.testSuites = malloc(5 * sizeof(TestSuite));
	testStatus.testSuitesSize = 5;
	testStatus.suiteCount = 0;
	addTestSuite(&testStatus, example1);
	addTestSuite(&testStatus, example2);
	addTestSuite(&testStatus, expected);
	addTestSuite(&testStatus, example3);
	TestSuite *actual = findSuite(&testStatus, testName);
	ASSERT_NON_NULL(actual);
	ASSERT_STRING_EQUALS(expected->name, actual->name);
	ASSERT_LONG_EQUALS(expected->testCount, actual->testCount);
	ASSERT_LONG_EQUALS(expected->testCaseSize, actual->testCaseSize);
	freeTestSuite(&example1);
	freeTestSuite(&example2);
	freeTestSuite(&example3);
	freeTestSuite(&expected);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, findTestSuiteNullSuiteName, Tests the findSuite function when the suite name is null)
{
	TestStatus testStatus;
	testStatus.testSuites = malloc(sizeof(TestSuite));
	ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	TestSuite *actual = findSuite(&testStatus, NULL);
	ASSERT_NULL(actual);
	free(testStatus.testSuites);
}
