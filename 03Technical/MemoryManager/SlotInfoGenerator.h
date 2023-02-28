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

	// for recycle
	static SlotInfoChunk *s_pSlotInfoChunktHead;
	static void GenerateSlotInfoChunks();

	// static SlotInfo** s_apSlotInfo;
	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

public:
	SlotInfoGenerator(int nClassId = _SlotInfoGenerator_Id,
		const char* pClassName = _SlotInfoGenerator_Name);
	virtual ~SlotInfoGenerator();
	virtual void Initialize();
	virtual void Finalize();

	virtual void* Malloc(size_t szObject, const char* sMessage);
	virtual bool Free(void* pObject);

	// maintenance
	virtual void Show(const char* sMessage);
};

