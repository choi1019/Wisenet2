#include <13PTechnical/PMemoryManager/PSlotList.h>

PSlotList::PSlotList(
	size_t szSlotUnit,
	unsigned nClassId,
	const char* pcClassName)
	: SlotList(szSlotUnit, nClassId, pcClassName)
{
	pthread_mutex_init(&m_mutex, nullptr);
}
PSlotList::~PSlotList() {
//		DeleteCriticalSection(&CriticalSection);
	pthread_mutex_destroy(&m_mutex);
}

void PSlotList::Initialize() {
	SlotList::Initialize();
}
void PSlotList::Finalize() {
	SlotList::Finalize();
}

void PSlotList::Lock() {
//		EnterCriticalSection(&CriticalSection);
	pthread_mutex_lock(&m_mutex);
}
void PSlotList::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
	pthread_mutex_unlock(&m_mutex);
}