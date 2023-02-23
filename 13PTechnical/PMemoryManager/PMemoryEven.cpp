#include <13PTechnical/PMemoryManager/PMemoryEven.h>

PMemoryEven::PMemoryEven(
	size_t szSlotUnit,
	unsigned nClassId,
	const char* pcClassName)
	: MemoryEven(szSlotUnit, nClassId, pcClassName)
{
	pthread_mutex_init(&m_mutex, nullptr);
}
PMemoryEven::~PMemoryEven() {
//		DeleteCriticalSection(&CriticalSection);
	pthread_mutex_destroy(&m_mutex);
}

void PMemoryEven::Initialize() {
	MemoryEven::Initialize();
}
void PMemoryEven::Finalize() {
	MemoryEven::Finalize();
}

void PMemoryEven::Lock() {
//		EnterCriticalSection(&CriticalSection);
	pthread_mutex_lock(&m_mutex);
}
void PMemoryEven::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
	pthread_mutex_unlock(&m_mutex);
}