#pragma once

#include <03Technical/typedef.h>
#define _SlotListGenerator_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotListGenerator)
#define _SlotListGenerator_Name "SlotListGenerator"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class SlotListGenerator : public MemoryObject {
public:
	enum class EException {
		eBegin = _SlotListGenerator_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	static PageIndex *s_pPageIndexHead;

	// static SlotInfo** s_apSlotInfo;
	void* operator new(size_t szThis, void* pMemoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, const char* sMessage);

public:
	SlotListGenerator(int nClassId = _SlotListGenerator_Id,
		const char* pClassName = _SlotListGenerator_Name);
	~SlotListGenerator();
	void Initialize();
	void Finalize();

	PageIndex *AllocateAPage();
	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);

	// maintenance
	void Show(const char* sMessage);
};

