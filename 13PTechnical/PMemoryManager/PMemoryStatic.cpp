#include <13PTechnical/PMemoryManager/PMemoryStatic.h>

void PMemoryStatic::Lock() {
//		EnterCriticalSection(&CriticalSection);
	pthread_mutex_lock(&m_mutex);
}
void PMemoryStatic::UnLock() {
//		LeaveCriticalSection(&CriticalSection);
	pthread_mutex_unlock(&m_mutex);
}

PMemoryStatic::PMemoryStatic(
	unsigned nClassId,
	const char* pcClassName)
	: MemoryStatic(nClassId, pcClassName)
{
	pthread_mutex_init(&m_mutex, nullptr);
}
PMemoryStatic::~PMemoryStatic() {
//		DeleteCriticalSection(&CriticalSection);
	pthread_mutex_destroy(&m_mutex);
}

void PMemoryStatic::Initialize() {
	MemoryStatic::Initialize();
}
void PMemoryStatic::Finalize() {
	MemoryStatic::Finalize();
}


void* PMemoryStatic::Malloc(size_t szObject, const char* sMessage)
{
    Lock();
    void* pObject = MemoryStatic::Malloc(szObject, sMessage);
    UnLock();
    return pObject;
}

bool PMemoryStatic::Free(void* pObject) {
    Lock();
    bool result = MemoryStatic::Free(pObject);
    UnLock();
    return result;
}