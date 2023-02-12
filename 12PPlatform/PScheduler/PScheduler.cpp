#include <12PPlatform/PScheduler/PScheduler.h>
#include <01Base/Aspect/Exception.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>

void* CallBackPScheduler(void *pObject) {
	PScheduler *pPScheduler = (PScheduler *)pObject;
	pPScheduler->Run();
	return nullptr;
}

void PScheduler::Fork() {
	m_idThared = pthread_create((&m_thread), NULL, CallBackPScheduler, (void*)this);
	if(m_idThared < 0) {
		throw Exception();
	}
}

void PScheduler::Join() {
	pthread_join(m_thread, (void**)&m_stsThread);
}

PScheduler::PScheduler(
	int uClassId,
	const char* pcClassName)
	: Scheduler(uClassId, pcClassName)
{
	this->SetPEventQueue(new("PEventQueue") PEventQueue());
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
