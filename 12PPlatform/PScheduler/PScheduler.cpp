#include <12PPlatform/PScheduler/PScheduler.h>
#include <01Base/Aspect/Exception.h>

void* CallBackPScheduler(void *pObject) {
	PScheduler *pPScheduler = (PScheduler *)pObject;
	pPScheduler->Run();
	return nullptr;
}

void PScheduler::Start() {
	m_idThared = pthread_create((&m_thread), NULL, CallBackPScheduler, (void*)this);
	if(m_idThared < 0) {
		throw Exception();
	}
}

void PScheduler::Stop() {
	pthread_join(m_thread, (void**)&m_stsThread);
}

PScheduler::PScheduler(
	int uClassId,
	const char* pcClassName)
	: Scheduler(uClassId, pcClassName)
{
}
PScheduler::~PScheduler() 
{
}

void PScheduler::InitializeAsAScheduler(int uPriority) {
	Scheduler::InitializeAsAScheduler(uPriority);
}

void PScheduler::FinalizeAsAScheduler() {
	Scheduler::FinalizeAsAScheduler();

}



void PScheduler::Initialize() {
	Scheduler::Initialize();
}
void PScheduler::Finalize() {
	Scheduler::Finalize();
}
