#pragma once

#include <03Technical/typedef.h>
#define _MemoryEven_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryEven_Name "MemoryEven"

#include <03Technical/MemoryManager/SlotList.h>

class MemoryEven : public SlotList {
	void* Malloc(size_t szSlot, const char* sMessage) override {
		return SlotList::Malloc(szSlot, sMessage);
	}
	bool Free(void* pObject) override {
		return SlotList::Free(pObject);
	}
	// critical section
	virtual void Lock() {};
	virtual void UnLock() {};

public:
	// for head MemoryEven
	MemoryEven(size_t szSlot, 
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name)
		: SlotList(szSlot, nClassId, pClassName) 
		{
		}
	// for normal MemoryEven
	MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name)
		: SlotList(szSlot, numMaxSlots, numPagesRequired, pSlotListHead, nClassId, pClassName) 
		{
		}
	virtual ~MemoryEven(){}
	virtual void Initialize() {
		SlotList::Initialize();
	}
	virtual void Finalize() {
		SlotList::Finalize();
	}

	// maintenance
	virtual void Show(const char* pTitle) {
		SlotList::Show(pTitle);
	}
};

