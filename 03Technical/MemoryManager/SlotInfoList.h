#pragma once

#include <03Technical/typedef.h>
#define _SlotInfoList_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotInfoList)
#define _SlotInfoList_Name "SlotInfoList"

#include <01Base/Memory/IMemory.h>
#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotInfo.h>

class SlotInfoList : public MemoryObject, public IMemory {
public:
	enum class EException {
		eBegin = _SlotInfoList_Id,
		eNoMoreSlotInfo,
		eFreeError,
		eEnd
	};
	
	// for recycle
	static SlotInfoList* s_pSlotInfoListRecycle;
	static PageList* s_pPageList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	int m_szPage;
	int m_numPagesRequired;
	int m_idxPage;
	PageIndex* m_pPageIndex;

	size_t m_szSlotInfo;
	unsigned m_numMaxSlotInfos;
	unsigned m_numSlotInfos;

	SlotInfo* m_pSlotInfoHead;
	bool m_bGarbage;

	int m_nCountSlotInfoLists;
	SlotInfoList *m_pSlotInfoListHead;
	SlotInfoList* m_pNext;
	SlotInfoList* m_pSibling;
	
	void* Malloc(size_t szSlotInfo, const char* sMessage);
	bool Free(void* pObject);

protected:
	// critical section
	virtual void Lock() {};
	virtual void UnLock() {};

public:
	// getters and setters
	size_t GetSzSlotInfo() { return this->m_szSlotInfo; }
	unsigned GetNumSlotInfos() { return this->m_numSlotInfos; }

	int GetIdxPage() { return this->m_idxPage; }
	bool IsGarbage() { return this->m_bGarbage; }
	int GetCountSlotInfoLists() { return this->m_nCountSlotInfoLists; }

	SlotInfoList* GetPNext() { return this->m_pNext; }
	void SetPNext(SlotInfoList* pNext) { this->m_pNext = pNext; }
	SlotInfoList* GetPSibling() { return this->m_pSibling; }
	void SetPSibling(SlotInfoList* pSibling) { this->m_pSibling = pSibling; }
//	PageIndex* GetPPageIndex() { return this->m_pPageIndex; }
	SlotInfo *GetASlotInfo();
	void PutASlotInfo(SlotInfo *pSlotInfo);

public:
	// for head SlotInfoList
	SlotInfoList(size_t szSlotInfo, 
		int nClassId = _SlotInfoList_Id,
		const char* pClassName = _SlotInfoList_Name);
	// for normal SlotInfoList
	SlotInfoList(size_t szSlotInfo, int numMaxSlotInfos, int szPage, int numPagesRequired, SlotInfoList *pSlotInfoListHead,
		int nClassId = _SlotInfoList_Id,
		const char* pClassName = _SlotInfoList_Name);
	virtual ~SlotInfoList();
	virtual void Initialize();
	virtual void Finalize();

	// SlotInfo* AllocateASlotInfo(size_t szAllocate, const char* sMessage);
	// bool FreeASlotInfo(SlotInfo* pSlotInfoFree);
	// methods
	void* SafeMalloc(size_t szAllocate, const char* sMessage) override;
	bool SafeFree(void* pObject) override;

	// maintenance
	virtual void Show(const char* pTitle);
};

