#pragma once

#include <91TestPlatform/typedef.h>
#define _TestObject_Id _GET_TESTCLASS_UID(_ELayer_TestPlatform::_eTestObject)
#define _TestObject_Name "TestObject"

class TestObject
{
public:
	// static members
	void* operator new(size_t szThis, const char* sMessage) {
		void* pObject = malloc(szThis);
    	return pObject;
	}
	void operator delete(void* pObject) {
	//   TESTLOG_NEWLINE("#TestObject::free (pObject)", (size_t)pObject);
		free(pObject);
	}
	void operator delete(void* pObject, const char* sMessage) {
//		throw TestException(1, "TestObject", "delete", "not support");
	}

private:
	unsigned m_nClassId;
	const char* m_pcClassName;
	
public:
	// getters and setters
	inline unsigned GetClassId() { return this->m_nClassId; }
	inline const char* GetClassName() { return this->m_pcClassName; }

public:
	TestObject(
		unsigned nClassId = _TestObject_Id,
		const char *pcClassName = _TestObject_Name)
		: m_nClassId(nClassId)
		, m_pcClassName(pcClassName)
		{}
	virtual ~TestObject() {}
};

