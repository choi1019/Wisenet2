#pragma once

#include <01Base/typedef.h>
#define _IMemory_Id _GET_CLASS_UID(_ELayer_Base::_eIMemory)
#define _IMemoryName "IMemory"

class SlotInfo;
class IMemory
{
public:
	enum class EException
	{
		_eBegin = _IMemory_Id,
		_eNoMorePage,
		_eNoMoreSlot,
		_eNoMoreSlotList,
		_eNoMoreStaticMemory,

		_eSlotAllocationFailed,
		_eSlotListAllocationFailed,

		_eSlotDelocationFailed,
		_eSlotListDelocationFailed,

		_eNotSupport,
		_eEnd
	};

	// methods
	virtual void* Malloc(size_t szAllocate, const char* pcName) = 0;
	virtual bool Free(void* pPtr) = 0;

	virtual SlotInfo* GetPSlotInfo(void *pObject) { return nullptr; }
	virtual void Show(const char* pTitle) = 0;
};