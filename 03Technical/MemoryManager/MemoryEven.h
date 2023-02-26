#pragma once

#include <03Technical/typedef.h>
#define _MemoryEven_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryEven)
#define _MemoryEven_Name "MemoryEven"

#include <03Technical/MemoryManager/SlotList.h>

class MemoryEven : public SlotList {
private:
	void AddSlotInfo(Slot *pSlot, const char *sMessage);
	void DeleteSlotInfo(Slot *pSlot);

public:
	// for head MemoryEven
	MemoryEven(size_t szSlot, 
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name);
	// for normal MemoryEven
	MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
		int nClassId = _MemoryEven_Id,
		const char* pClassName = _MemoryEven_Name);
	virtual ~MemoryEven();
	virtual void Initialize();
	virtual void Finalize();
	
	SlotInfo* GetPSlotInfoHead() { return this->m_pSlotInfoHead; }
	SlotInfo *GetPSlotInfo(void *pObject);

	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);

	// maintenance
	virtual void Show(const char* pTitle);
};

