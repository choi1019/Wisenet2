#pragma once

#include <91TestPlatform/typedef.h>
#define _TestMain_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestMain)
#define _TestMain_Name "TestMain"

#include <91TestPlatform/TestCase/TestSuite.h>

class TestMain: public TestSuite {
public:
	TestMain(unsigned nClassId = _TestMain_Id, const char* pClassName = _TestMain_Name)
	: TestSuite(nClassId, pClassName)
	 {}
	virtual ~TestMain() {}

	virtual void BeforeInitialize() { 
		// TESTLOG_NEWLINE("");
		TestSuite::BeforeInitialize();
	}
	virtual void AfterFinalize() {
		TestSuite::AfterFinalize();
		TESTLOG_NEWLINE("");
	}
};