#include <13PTechnical/PThread/PThread.h>
#include <01Base/Aspect/Exception.h>

void* CallBackPThread(void *pObject) {
	PThread *pThread = (PThread *)pObject;
	pThread->RunThread();
	return nullptr;
}

void PThread::Fork() {
	m_idThared = pthread_create((&m_thread), NULL, CallBackPThread, (void*)this);
	if(m_idThared < 0) {
		throw Exception();
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