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
	static void* 	s_pMemeoryAllocated;
	static size_t 	s_szMemoryAllocated;
	static void* 	s_pMemeoryCurrent;
	static size_t 	s_szMemoryCurrent;

	void* operator new(size_t szThis, void *s_pMemeoryAllocated, size_t s_szMemoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void *s_pMemeoryAllocated, size_t s_szMemoryAllocated, const char* sMessage);

private:
	size_t m_szPage;
	unsigned m_numPagesAllocated;
	unsigned m_numPagesCurrent;
	PageIndex** m_apPageIndices;
	void *m_pStartPage;
	
public:
	size_t GetSzPage() { return this->m_szPage; }
	unsigned GetNumPagesCurrent() { return this->m_numPagesCurrent; }
	unsigned GetNumPagesAllocated() { return this->m_numPagesAllocated; }
	int GetIdxPage(void *pObject) { 
			return ((size_t)pObject - (size_t)m_pStartPage) / m_szPage; 
	}
	PageIndex *GetPPageIndex(void *pObject) { return m_apPageIndices[GetIdxPage(pObject)]; }

public:
	PageList(
		size_t szPage,
		int nClassId = _PageList_Id,
		const char* pClassName = _PageList_Name);
	virtual ~PageList();
	virtual void Initialize();
	virtual void Finalize();

	PageIndex* AllocatePages(unsigned numPagesRequired, SlotList *pSlotList);
	void DelocatePages(unsigned indexFree);
	void Show(const char* pTitle);

	void* Malloc(size_t szObject, const char* sMessage) override { return nullptr; }
	bool Free(void* pObject) override { return false; }
};

