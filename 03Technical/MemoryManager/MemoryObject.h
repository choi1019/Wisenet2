#pragma once

#include <03Technical/typedef.h>
#define _MemoryObject_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryObject)
#define _MemoryObject_Name "MemoryObject"

#include <01Base/Object/BaseObject.h>
#include <01Base/Memory/IMemory.h>

class MemoryObject : public BaseObject, public IMemory { 
public:
	MemoryObject(
		int nClassId = _MemoryObject_Id,
		const char* pClassName = _MemoryObject_Name)
		: BaseObject(nClassId, pClassName)
	{
	}
	virtual ~MemoryObject() {
	}
	virtual void Initialize() {
	}
	virtual void Finalize() {
	}
};