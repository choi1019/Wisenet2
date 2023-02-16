#include <13PTechnical/PThread/PThread.h>
#include <01Base/Aspect/Exception.h>

// static void* CallBackPThread(void *pObject) {
// 	PThread *pThread = (PThread *)pObject;
// 	pThread->RunThread();
// 	return nullptr;
// }

void PThread::Fork(void *(*__start_routine)(void *), void *pObject) {
	m_idThared = pthread_create((&m_thread), NULL, __start_routine, pObject);
	// m_idThared = pthread_create((&m_thread), NULL, CallBackPThread, pObject);
	if(m_idThared < 0) {
		throw Exception(1, "PThread::Fork");
	}
}

void PThread::Join() {
	pthread_join(m_thread, (void**)&m_stsThread);
}

PThread::PThread()
{
}
PThread::~PThread() 
{
}