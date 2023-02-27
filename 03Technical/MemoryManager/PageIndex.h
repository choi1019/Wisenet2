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
	unsigned m_index;
	Page* m_pPage;

	bool m_bAllocated;
	unsigned m_numAllocated;
	SlotList *m_pSlotList;

public:
	unsigned GetIndex() { return this->m_index; }
	Page* GetPPage() { return this->m_pPage; }
	bool IsAllocated() { return this->m_bAllocated; }
	void SetIsAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }
	unsigned GetNumAllocated() { return this->m_numAllocated; }
	void SetNumAllocated(size_t numAllocated) { this->m_numAllocated = numAllocated; }
	void SetPSlotList(SlotList *pSlotList) { m_pSlotList = pSlotList; }
	SlotList *GetPSlotList() { return this->m_pSlotList; }

public:
	PageIndex(
		unsigned index,
		void* pMemoryAllocated,
	 	int nClassId = _PageIndex_Id,
		const char* pClassName = _PageIndex_Name);
	virtual ~PageIndex();
	virtual void Initialize();
	virtual void Finalize();

	void* Malloc(size_t szObject, const char* sMessage) override { return nullptr; }
	bool Free(void* pObject) override { return false; }
	
	void Show(const char* pTitle);
};
