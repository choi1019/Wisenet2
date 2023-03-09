#pragma once

#include <03Technical/typedef.h>
#define _SlotGenerator_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotGenerator)
#define _SlotGenerator_Name "SlotGenerator"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class SlotGenerator : public MemoryObject {
public:
	enum class EException {
		eBegin = _SlotGenerator_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	static PageList *s_pPageList;	

	// static Slot** s_apSlot;
	void* operator new(size_t szThis, void* PMemoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* PMemoryAllocated, const char* sMessage);
	
private:
	PageIndex *m_pPageIndexHead;

public:
	SlotGenerator(int nClassId = _SlotGenerator_Id,
		const char* pClassName = _SlotGenerator_Name);
	~SlotGenerator();
	void Initialize();
	void Finalize();
	
	PageIndex *AllocateAPage(size_t szSlot);
	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);

	// maintenance
	void Show(const char* sMessage);
};

