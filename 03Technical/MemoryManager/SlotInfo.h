#pragma once

#include <03Technical/typedef.h>
#define _SlotInfo_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotInfo)
#define _SlotInfo_Name "SlotInfo"

#include <01Base/Memory/IMemory.h>

class SlotInfo
{
public:
	static IMemory *s_pMemory;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	// dummy
	void operator delete(void* pObject, const char* sMessage);

public:
	SlotInfo() {}
	virtual ~SlotInfo() {}
	virtual void Initialize() {}
	virtual void Finalize() {}

	// maintenance
	virtual void Show(const char* pTitle);
};