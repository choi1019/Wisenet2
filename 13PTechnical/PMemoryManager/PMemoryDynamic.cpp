#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

	PMemoryDynamic::PMemoryDynamic(
		size_t szSlotUnit,
		unsigned nClassId,
		const char* pcClassName)
		: MemoryDynamic(szSlotUnit, nClassId, pcClassName)
	{
//		InitializeCriticalSection(&CriticalSection);
        pthread_mutex_init(&m_mutex, nullptr);
	}
	PMemoryDynamic::~PMemoryDynamic() {
//		DeleteCriticalSection(&CriticalSection);
        pthread_mutex_destroy(&m_mutex);
	}

	void PMemoryDynamic::Initialize() {
		MemoryDynamic::Initialize();
	}
	void PMemoryDynamic::Finalize() {
		MemoryDynamic::Finalize();
	}

	void PMemoryDynamic::Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void PMemoryDynamic::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}