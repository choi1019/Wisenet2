#include "PScheduler.h"

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

void PScheduler::Start() {
}

void PScheduler::Join() {
}

void PScheduler::Initialize() {
	Scheduler::Initialize();
}
void PScheduler::Finalize() {
	Scheduler::Finalize();
}
