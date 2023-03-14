#pragma once

#include <03Technical/typedef.h>
#define _SlotInfo_Id _GET_CLASS_UID(_ELayer_Technical::_eSlotInfo)
#define _SlotInfo_Name "SlotInfo"

#include <01Base/Memory/IMemory.h>
#include <01Base/Object/ValueObject.h>
class SlotList;
class Slot;
class PageList;

class SlotInfo : public ValueObject
{
public:
	static IMemory *s_pMemory;
	static PageList *s_pPageList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);
	
private:
	Slot *m_pSlot;
	char m_sMessage[50];
	SlotList *m_pSlotList;
	SlotInfo *m_pNext;
public:
	Slot *GetPSlot() { return this->m_pSlot; }
	char* GetSMessage() { return this->m_sMessage; }
	SlotList *GetPSlotList() { return this->m_pSlotList; }
	SlotInfo *GetPNext() { return this->m_pNext; }
	void SetPNext(SlotInfo *pNext) { this->m_pNext = pNext; }

public:
	SlotInfo(Slot *pSlot, const char *sMessage, SlotList *pSlotList,
			int nObjectId = _SlotInfo_Id, const char* sObjectName = _SlotInfo_Name
	);
	~SlotInfo() override;

	// maintenance
	void Show(const char* pTitle);
};