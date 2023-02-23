#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

PMemoryDynamic::PMemoryDynamic(
	size_t szSlotUnit,
	unsigned nClassId,
	const char* pcClassName)
	: MemoryDynamic(szSlotUnit, nClassId, pcClassName)
{
	pthread_mutex_init(&m_mutex, nullptr);
}
PMemoryDynamic::~PMemoryDynamic() {
	pthread_mutex_destroy(&m_mutex);
}

void PMemoryDynamic::Initialize() {
	MemoryDynamic::Initialize();
}
void PMemoryDynamic::Finalize() {
	MemoryDynamic::Finalize();
}

void PMemoryDynamic::Lock() {
	pthread_mutex_lock(&m_mutex);
}
void PMemoryDynamic::UnLock() {
	pthread_mutex_unlock(&m_mutex);
}