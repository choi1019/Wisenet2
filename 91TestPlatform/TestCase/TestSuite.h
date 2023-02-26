#pragma once

#include <91TestPlatform/typedef.h>
#define _TestSuite_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestSuite)
#define _TestSuite_Name "TestSuite"

#include <vector>
#include <91TestPlatform/TestCase/TestCase.h>

class TestSuite : public TestCase
{
protected:
	vector<TestCase*> m_vPTestObjects;

	void Add(TestCase* pTestCase) {
		this->m_vPTestObjects.push_back(pTestCase);
	}
	void DeleteTestCases() {
		for (TestCase* pTestCase : m_vPTestObjects) {
        	delete pTestCase;
    	}
	}
	
public:
	TestSuite(
		unsigned nClassId = _TestSuite_Id, 
		const char* pClassName = _TestSuite_Name)
		: TestCase(nClassId, pClassName)
		, m_vPTestObjects()
	{}
	virtual ~TestSuite() {}

	virtual void Initialize() {
	}
	virtual void Finalize() {
		DeleteTestCases();
	}
	virtual void Run() {
		try {
			for (TestCase* pTestCase: m_vPTestObjects) {
				pTestCase->BeforeInitialize();
				pTestCase->Initialize();
				pTestCase->BeforeRun();
				pTestCase->Run();
				pTestCase->AfterRun();
				pTestCase->Finalize();
				pTestCase->AfterFinalize();
			}
		}
		catch (TestException& exception) {
			exception.Println();
		}
	}
};

