/**
 * CTest unit testing library 
 * @Author Miguel Blanco Godón
 */

#include "../../include/ctest_api.h"
#include "../../src/ctest_core.h"

TEST(TestSuitesTests, createTestSuite, Tests the creation of a CTest_TestSuite element)
{
	const char * expectedSuiteName = "testSuite";
	CTest_TestSuite *actual = createTestSuite(expectedSuiteName);
	CT_ASSERT_STRING_EQUALS(expectedSuiteName, actual->name);
	CT_ASSERT_NON_NULL(actual->testCases);
	CT_ASSERT_LONG_EQUALS(10, actual->testCaseSize);
	CT_ASSERT_LONG_EQUALS(0, actual->testCount);
	CT_ASSERT_LONG_EQUALS(0, actual->successfulTests);
	CT_ASSERT_LONG_EQUALS(0, actual->failedTests);
	free(actual->testCases);
	actual->testCases = NULL;
	free(actual);
	actual = NULL;
}

TEST(TestSuitesTests, freeTestSuite, Tests the liberation of a CTest_TestSuite element)
{
	CTest_TestSuite *testSuite = malloc(sizeof(CTest_TestSuite));
	testSuite->testCount = 0;
	CT_ASSERT_NON_NULL(testSuite);
	testSuite->testCases = malloc(sizeof(CTest_TestCase));
	CT_ASSERT_NON_NULL(testSuite->testCases);
	freeTestSuite(&testSuite);
}

TEST(TestSuitesTests, freeTestSuiteWithNullTestCases, Tests the liberation of a CTest_TestSuite element if the inner TestCase pointer is NULL)
{
	CTest_TestSuite *testSuite = malloc(sizeof(CTest_TestSuite));
	CT_ASSERT_NON_NULL(testSuite);
	testSuite->testCases = NULL;
	CT_ASSERT_NULL(testSuite->testCases);
	freeTestSuite(&testSuite);
	CT_ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, freeNullTestSuite, Tests the liberation of a CTest_TestSuite element does not double free a NULL TestSuite)
{
	CTest_TestSuite *testSuite = NULL;
	CT_ASSERT_NULL(testSuite);
	freeTestSuite(&testSuite);
	CT_ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, addTestSuite, Tests the addition of a TestSuite)
{
	const char *testName = "test";
	CTest_TestStatus testStatus;
	testStatus.testSuites = malloc(sizeof(CTest_TestSuite));
	CT_ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	CTest_TestSuite *testSuite = createTestSuite(testName);
	CT_ASSERT_NON_NULL(testSuite);
	addTestSuite(&testStatus, testSuite);
	CT_ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 1);
	CT_ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	CT_ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	CT_ASSERT_NULL(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, addTestSuiteDuplicate, Tests the addition of a CTest_TestSuite twice)
{
	const char *testName = "test";
	CTest_TestStatus testStatus;
	testStatus.testSuites = malloc(2* sizeof(CTest_TestSuite));
	CT_ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 2;
	testStatus.suiteCount = 0;
	CTest_TestSuite *testSuite = createTestSuite(testName);
	CT_ASSERT_NON_NULL(testSuite);
	addTestSuite(&testStatus, testSuite);
	CT_ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	CT_ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	CT_ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	addTestSuite(&testStatus, testSuite);
	CT_ASSERT_LONG_EQUALS(testStatus.testSuitesSize, 2);
	CT_ASSERT_LONG_EQUALS(testStatus.suiteCount, 1);
	CT_ASSERT_STRING_EQUALS(testSuite->name, testStatus.testSuites[0]->name);
	freeTestSuite(&testSuite);
	CT_ASSERT_NULL(testSuite);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, findTestSuiteNullTestStatus, Tests the findSuite does not crash when teh testStatus is NULL)
{
	const char *suiteName = "test";
	CTest_TestSuite *testSuite = findSuite(NULL, suiteName);
	CT_ASSERT_NULL(testSuite);
}

TEST(TestSuitesTests, findTestSuite, Tests the CTest_TestSuite element search function by name over the TestStatus element)
{
	const char *testName = "test";
	const char *example1Name = "example1";
	const char *example2Name = "example2";
	const char *example3Name = "example3";
	CTest_TestSuite *expected = createTestSuite(testName);
	CTest_TestSuite *example1 = createTestSuite(example1Name);
	CTest_TestSuite *example2 = createTestSuite(example2Name);
	CTest_TestSuite *example3 = createTestSuite(example3Name);
	CTest_TestStatus testStatus;
	testStatus.testSuites = malloc(5 * sizeof(CTest_TestSuite));
	testStatus.testSuitesSize = 5;
	testStatus.suiteCount = 0;
	addTestSuite(&testStatus, example1);
	addTestSuite(&testStatus, example2);
	addTestSuite(&testStatus, expected);
	addTestSuite(&testStatus, example3);
	CTest_TestSuite *actual = findSuite(&testStatus, testName);
	CT_ASSERT_NON_NULL(actual);
	CT_ASSERT_STRING_EQUALS(expected->name, actual->name);
	CT_ASSERT_LONG_EQUALS(expected->testCount, actual->testCount);
	CT_ASSERT_LONG_EQUALS(expected->testCaseSize, actual->testCaseSize);
	freeTestSuite(&example1);
	freeTestSuite(&example2);
	freeTestSuite(&example3);
	freeTestSuite(&expected);
	free(testStatus.testSuites);
}

TEST(TestSuitesTests, findTestSuiteNullSuiteName, Tests the findSuite function when the suite name is null)
{
	CTest_TestStatus testStatus;
	testStatus.testSuites = malloc(sizeof(CTest_TestSuite));
	CT_ASSERT_NON_NULL(testStatus.testSuites);
	testStatus.testSuitesSize = 1;
	testStatus.suiteCount = 0;
	CTest_TestSuite *actual = findSuite(&testStatus, NULL);
	CT_ASSERT_NULL(actual);
	free(testStatus.testSuites);
}
