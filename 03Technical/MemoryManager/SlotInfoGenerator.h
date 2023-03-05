#pragma once

#include <03Technical/typedef.h>
#define _SlotInfoGenerator_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotInfoGenerator)
#define _SlotInfoGenerator_Name "SlotInfoGenerator"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>

class SlotInfoChunk {
public:
	SlotInfoChunk* pNext;
};

class SlotInfoGenerator : public MemoryObject {
public:
	enum class EException {
		eBegin = _SlotInfoGenerator_Id,
		eNoMoreSlot,
		eFreeError,
		eEnd
	};

	static SlotInfoChunk *s_pSlotInfoChunktHead;

	// static SlotInfo** s_apSlotInfo;
	void* operator new(size_t szThis, void* PMemoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* PMemoryAllocated, const char* sMessage);

public:
	SlotInfoGenerator(int nClassId = _SlotInfoGenerator_Id,
		const char* pClassName = _SlotInfoGenerator_Name);
	~SlotInfoGenerator();
	void Initialize();
	void Finalize();
	void GenerateSlotInfoChunks();
	void* Malloc(size_t szObject, const char* sMessage);
	bool Free(void* pObject);

	// maintenance
	void Show(const char* sMessage);
};

