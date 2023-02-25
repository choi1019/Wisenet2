#pragma once

#include <03Technical/typedef.h>
#define _PageList_Id _GET_CLASS_UID(_ELayer_Technical::_ePageList)
#define _PageList_Name "PageList"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageIndex.h>

// class SlotList;
// class Slot;

class PageList : public MemoryObject {
private:
	size_t m_pMemeoryAllocated;
	size_t m_szPage;
	unsigned m_numPagesMax;
	unsigned m_numPagesAvaiable;

	// pageIndex
	PageIndex** m_apPageIndices;
	
public:
	size_t GetSzPage() { return this->m_szPage; }
	unsigned GetNumPagesAvailable() { return this->m_numPagesAvaiable; }
	unsigned GetNumPagesMax() { return this->m_numPagesMax; }
	int GetIdxPage(void *pObject) { return ((size_t)pObject - m_pMemeoryAllocated) / m_szPage; }

public:
	PageList(
		size_t pMemeoryAllocated, 
		size_t szMemoryAllocated, 
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

