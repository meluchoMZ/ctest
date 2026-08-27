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
	assertStringEquals(expectedSuiteName, actual->name);
	assertNonNull(actual->testCases);
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
	assertNonNull(testSuite);
	testSuite->testCases = malloc(sizeof(TestCase));
	assertNonNull(testSuite->testCases);
	freeTestSuite(&testSuite);
}

TEST(TestSuitesTests, freeTestSuiteWithNullTestCases, Tests the liberation of a TestSuite element if the inner TestCase pointer is NULL)
{
	TestSuite *testSuite = malloc(sizeof(TestSuite));
	assertNonNull(testSuite);
	testSuite->testCases = NULL;
	assertNull(testSuite->testCases);
	freeTestSuite(&testSuite);
	assertNull(testSuite);
}

TEST(TestSuitesTests, freeNullTestSuite, Tests the liberation of a TestSuite element does not double free a NULL TestSuite)
{
	TestSuite *testSuite = NULL;
	assertNull(testSuite);
	freeTestSuite(&testSuite);
	assertNull(testSuite);
}

TEST(TestSuitesTests, addTestSuite, Tests the addition of a TestSuite)
{
	const char *testName = "test";
	TestStatus testStatus;
	testStatus.testSuites = malloc(sizeof(TestSuite));
	assertNonNull(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	TestSuite *testSuite = createTestSuite(testName);
	assertNonNull(testSuite);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 1);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	assertStringEquals(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	assertNull(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, addTestSuiteDuplicate, Tests the addition of a TestSuite twice)
{
	const char *testName = "test";
	TestStatus testStatus;
	testStatus.testSuites = malloc(2* sizeof(TestSuite));
	assertNonNull(testStatus.testSuites);
	testStatus.testSuitesSize = 2;
	testStatus.suiteCount = 0;
	TestSuite *testSuite = createTestSuite(testName);
	assertNonNull(testSuite);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	assertStringEquals(testSuite->name, testStatus.testSuites[0]->name);
	addTestSuite(&testStatus, testSuite);
	ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	assertStringEquals(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	assertNull(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, findTestSuiteNullTestStatus, Tests the findSuite does not crash when teh testStatus is NULL)
{
	const char *suiteName = "test";
	TestSuite *testSuite = findSuite(NULL, suiteName);
	assertNull(testSuite);
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
	assertNonNull(actual);
	assertStringEquals(expected->name, actual->name);
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
	assertNonNull(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	TestSuite *actual = findSuite(&testStatus, NULL);
	assertNull(actual);
	free(testStatus.testSuites);
}
