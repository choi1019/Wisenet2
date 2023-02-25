#pragma once

#include <03Technical/typedef.h>
#define _SlotInfo_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotInfo)
#define _SlotInfo_Name "SlotInfo"

#include <01Base/Object/RootObject.h>
class MemoryEven;
class Slot;
class SlotInfo : public RootObject
{
public:
	static IMemory *s_pMemory;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);
	
private:
	Slot *m_pSlot;
	char m_sMessage[30];
	MemoryEven *m_pMemoryEven;
	SlotInfo *m_pNext;
public:
	Slot *GetPSlot() { return this->m_pSlot; }
	char* GetSMessage() { return this->m_sMessage; }
	MemoryEven *GetPMemoryEven() { return this->m_pMemoryEven; }
	SlotInfo *GetPNext() { return this->m_pNext; }
	void SetPNext(SlotInfo *pNext) { this->m_pNext = pNext; }

public:
	SlotInfo(Slot *pSlot, const char *sMessage, MemoryEven *pMemoryEven,
			int nObjectId = _SlotInfo_Id, const char* sObjectName = _SlotInfo_Name
	);
	~SlotInfo() override;
	void Initialize() override;
	void Finalize() override;

	// maintenance
	void Show(const char* pTitle);
};