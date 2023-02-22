#pragma once

#include <13PTechnical/typedef.h>
#define _PSlotList_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePSlotList)
#define _PSlotList_Name "PSlotList"

#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <pthread.h>

class PSlotList : public SlotList {
private:
//	CRITICAL_SECTION CriticalSection;
	pthread_mutex_t m_mutex;

protected:
	void Lock() override;
	void UnLock() override;

public:
	PSlotList(
		size_t szSlotUnit,
		unsigned nClassId = _PSlotList_Id,
		const char* pcClassName = _PSlotList_Name);
	~PSlotList() override;

	void Initialize() override;
	void Finalize() override;
};