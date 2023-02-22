#pragma once

#include <03Technical/typedef.h>
#define _SlotList_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotList)
#define _SlotList_Name "SlotList"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class Slot {
public:
	Slot* pNext;
};

class SlotList : public MemoryObject, public IMemory {
public:
	enum class EException {
		eBegin = _SlotList_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	// for recycle
	static SlotList* s_pSlotListRecycle;
	static PageList* s_pPageList;
	static SlotList** s_aPSlotList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	int m_szPage;
	int m_numPagesRequired;
	int m_idxPage;
	PageIndex* m_pPageIndex;

	size_t m_szSlot;
	unsigned m_numMaxSlots;
	unsigned m_numSlots;

	Slot* m_pSlotHead;
	bool m_bGarbage;

	int m_nCountSlotLists;
	SlotList *m_pSlotListHead;
	SlotList* m_pNext;
	SlotList* m_pSibling;
	void* Malloc(size_t szSlot, const char* sMessage);
	bool Free(void* pObject);

protected:
	// critical section
	virtual void Lock() {};
	virtual void UnLock() {};

public:
	// getters and setters
	size_t GetSzSlot() { return this->m_szSlot; }
	unsigned GetNumSlots() { return this->m_numSlots; }

	int GetIdxPage() { return this->m_idxPage; }
	bool IsGarbage() { return this->m_bGarbage; }
	int GetCountSlotLists() { return this->m_nCountSlotLists; }

	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }
//	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }
	Slot *GetASlot();
	void PutASlot(Slot *pSlot);

public:
	// for head SlotList
	SlotList(size_t szSlot, 
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	// for normal SlotList
	SlotList(size_t szSlot, int numMaxSlots, int szPage, int numPagesRequired, SlotList *pSlotListHead,
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	virtual ~SlotList();
	virtual void Initialize();
	virtual void Finalize();

	// Slot* AllocateASlot(size_t szAllocate, const char* sMessage);
	// bool FreeASlot(Slot* pSlotFree);
	// methods
	void* SafeMalloc(size_t szAllocate, const char* sMessage) override;
	bool SafeFree(void* pObject) override;

	// maintenance
	virtual void Show(const char* pTitle);
};

