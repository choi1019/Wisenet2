#pragma once

#include <03Technical/typedef.h>
#define _SlotList_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotList)
#define _SlotList_Name "SlotList"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotInfo.h>

class Slot {
public:
	Slot* pNext;
};

class SlotList : public MemoryObject {
public:
	enum class EException {
		eBegin = _SlotList_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	// for recycle
	static PageList* s_pPageList;
	static IMemory* s_pMemory;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	int m_numPagesRequired;
	int m_idxPage;
	PageIndex* m_pPageIndex;
	int m_nCountSlotLists;
	bool m_bGarbage;

	size_t m_szSlot;
	unsigned m_numMaxSlots;
	unsigned m_numSlots;
	Slot* m_pSlotHead;

	// head SlotList of the same size
	SlotList *m_pSlotListHead;
	SlotList* m_pSibling;
	SlotList* m_pNext;
protected:
	// slot info for MemoryEvent
	SlotInfo *m_pSlotInfoHead;

public:
	int GetNumPagesRequired() { return this->m_numPagesRequired; }
	int GetIdxPage() { return this->m_idxPage; }
	PageIndex *GetPPageIndex() { return this->m_pPageIndex; }
	// as a head SlotList
	int GetCountSlotLists() { return this->m_nCountSlotLists; }
	void SetCountSlotLists(int nCountSlotLists) { this->m_nCountSlotLists = nCountSlotLists; }

	size_t GetSzSlot() { return this->m_szSlot; }
	unsigned GetNumMaxSlots() { return this->m_numMaxSlots; }
	unsigned GetNumSlots() { return this->m_numSlots; }
	Slot* GetPSlotHead() { return this->m_pSlotHead; }

	bool IsGarbage() { return this->m_bGarbage; }
	SlotList* GetPSlotListHead() { return this->m_pSlotListHead;}
	SlotList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotList* pNext) { this->m_pNext = pNext; }
	SlotList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotList* pSibling) { this->m_pSibling = pSibling; }

	SlotInfo *GetPSlotInfoHead() { return m_pSlotInfoHead; }
	void SetPSlotInfoHead(SlotInfo *pSlotInfo) { m_pSlotInfoHead = pSlotInfo; }

	SlotInfo* FindSlotInfo(Slot* pSlot);
	void AllocateASlotInfo(Slot *pSlot, const char* sMessage);
	void DelocateASlotInfo(Slot *pSlot);

	Slot *AllocateASlot(const char* sMessage);
	void DelocateASlot(Slot *pSlot);




public:
	SlotList(size_t szSlot, SlotList *pSlotListHead,
		int nClassId = _SlotList_Id,
		const char* pClassName = _SlotList_Name);
	virtual ~SlotList();
	virtual void Initialize();
	virtual void Finalize();

	virtual void* Malloc(size_t szObject, const char* sMessage);
	virtual bool Free(void* pObject);

	// maintenance
	virtual void Show(const char* sMessage);
};

