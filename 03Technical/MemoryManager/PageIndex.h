#pragma once

#include <03Technical/typedef.h>
#define _PageIndex_Id _GET_CLASS_UID(_ELayer_Technical::_ePageIndex)
#define _PageIndex_Name "PageIndex"

#include <03Technical/MemoryManager/MemoryObject.h>

class SlotList;
class Page {
public:
};

class PageIndex : public MemoryObject {
public:
	void* operator new(size_t szThis, void *s_pMemeoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void *s_pMemeoryAllocated, const char* sMessage);
	
private:
	class Slot {
		public:
			Slot* m_pNext;
	};

	unsigned m_index;
	Page* m_pPage;

	bool m_bAllocated;
	unsigned m_numAllocated;

	SlotList *m_pSlotList;
	PageIndex *m_pSibling;

	int m_numSlotsAllocated;
	int m_numSlotsCurrent;
	void *m_pSlotHead;

public:
	unsigned GetIndex() { return this->m_index; }
	Page* GetPPage() { return this->m_pPage; }
	bool IsAllocated() { return this->m_bAllocated; }
	void SetIsAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }
	unsigned GetNumAllocated() { return this->m_numAllocated; }
	void SetNumAllocated(size_t numAllocated) { this->m_numAllocated = numAllocated; }

	// for generators
	void SetPSlotList(SlotList *pSlotList) { m_pSlotList = pSlotList; }
	SlotList *GetPSlotList() { return this->m_pSlotList; }
	void *GetPSlotHead() { return this->m_pSlotHead; }
	void SetPSlotHead(void *pSlotHead) { m_pSlotHead = pSlotHead; }
	void SetPSibling(PageIndex *pSibling) { m_pSibling = pSibling; }
	PageIndex *GetPSibling() { return this->m_pSibling; }

	void* GenerateSlots(int szSlot) {
		int szPage = SlotInfo::s_pPageList->GetSzPage();
		int numMaxSlotInfoChunks = szPage / szSlot;
		// for SlotInfo
		// pPageIndex->SetNumSlots(numMaxSlotInfoChunks);

		Slot* pPrevious = nullptr;
		Slot* pCurrent = (Slot*)this->GetPPage();
		for (int i = 0; i < numMaxSlotInfoChunks; i++) {
			pCurrent->m_pNext = (Slot*)((size_t)pCurrent + szSlot);
			pPrevious = pCurrent;
			pCurrent = pCurrent->m_pNext;
		}
		pPrevious->m_pNext = nullptr;
	}

	void SetNumSlots(int numSlots) { 
		m_numSlotsAllocated = numSlots;
		m_numSlotsCurrent = m_numSlotsAllocated; 
	}
	int GetNumSlotsAllocated() { return m_numSlotsAllocated; }
	int GetNumSlotsAvailable() { return m_numSlotsCurrent; }
	void AllocateASlot() { --m_numSlotsCurrent; }
	void DelocateASlot() { ++m_numSlotsCurrent;	}
	bool IsGarbage() { return (m_numSlotsCurrent == m_numSlotsAllocated)? true: false; }

public:
	PageIndex(
	 	int nClassId = _PageIndex_Id,
		const char* pClassName = _PageIndex_Name);
	virtual ~PageIndex();
	virtual void Initialize(unsigned indexPage, void* pPage);
	virtual void Finalize();

	void* Malloc(size_t szObject, const char* sMessage) override { return nullptr; }
	bool Free(void* pObject) override { return false; }
	
	void Show(const char* pTitle);
};
