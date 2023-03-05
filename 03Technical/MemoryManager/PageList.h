#pragma once

#include <03Technical/typedef.h>
#define _PageList_Id _GET_CLASS_UID(_ELayer_Technical::_ePageList)
#define _PageList_Name "PageList"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageIndex.h>

// class SlotList;
// class Slot;

class PageList : public MemoryObject {
public:
	void* operator new(size_t szThis, void *pMemoryCurrent, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void *pMemoryCurrent, const char* sMessage);

private:
	void *m_pMemoryAllocated;
	size_t m_szMemoryAllocated;
	void *m_pMemoryCurrent;
	size_t m_szMemoryCurrent;
	
	size_t m_szPage;
	unsigned m_numPagesAllocated;
	unsigned m_numPagesCurrent;
	void *m_pPageHead;

	PageIndex** m_apPageIndices;
		
public:
	size_t GetSzPage() { return this->m_szPage; }
	unsigned GetNumPagesCurrent() { return this->m_numPagesCurrent; }
	unsigned GetNumPagesAllocated() { return this->m_numPagesAllocated; }
	int GetIdxPage(void *pObject) { 
			return ((size_t)pObject - (size_t)m_pPageHead) / m_szPage;	}
	PageIndex *GetPPageIndex(void *pObject) { return m_apPageIndices[GetIdxPage(pObject)]; }

public:
	PageList(
		
		int nClassId = _PageList_Id,
		const char* pClassName = _PageList_Name);
	virtual ~PageList();
	virtual size_t Initialize(void *pMemoryCurrent, size_t szMemoryCurrent, size_t szPage);
	virtual void Finalize();

	PageIndex* AllocatePages(unsigned numPagesRequired, SlotList *pSlotList);
	void DelocatePages(unsigned indexFree);
	void Show(const char* pTitle);

	void* Malloc(size_t szObject, const char* sMessage) override { return nullptr; }
	bool Free(void* pObject) override { return false; }
};

