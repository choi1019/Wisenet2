#pragma once

#include <91TestPlatform/typedef.h>
#define _TestException_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestException)
#define _TestException_Name "TestException"

#include <91TestPlatform/TestObject/TestObject.h>

class TestException : public TestObject {
private:
	unsigned m_nType;
	string m_sObject;
	string m_sFunction;
	string m_sMessage;
public:
	TestException(
		unsigned nType,
		string sObject,
		string sFunction,
		string sMessage,
		unsigned nClassId = _TestException_Id, const char* pcClassName = _TestException_Name)
//     : TestObject(nClassId, pcClassName)
    : m_nType(nType)
    , m_sObject(sObject)
    , m_sFunction(sFunction)
    , m_sMessage(sMessage)
	{}

	~TestException() {}

	void Initialize() {}
	void Finalize() {}
	void Println() {
		printf(
			">> TestException(%d) %s::%s %s\n"
			, m_nType
			, m_sObject.c_str()
			, m_sFunction.c_str()
			, m_sMessage.c_str()
			//			,String(m_tTime).c_str()
		);
	}
};