#include <19PApplication/PMain/PMain.h>

#include <19PApplication/PMain/Config.h>
#include <13PTechnical/PMemoryManager/PMemoryManager.h>
#include <19PApplication/PLifecycleManager/PLifecycleManager.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>

#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

PMain::PMain(unsigned uClassId,
	const char* pcClassName)
	: Main(uClassId, pcClassName)
{
}
PMain::~PMain() {
}

void PMain::BootstrapSystem() {
	LOG_HEADER0();

	// Main EventQueue
	PEventQueue* pPEventQueue = new("PMain::PEventQueue") PEventQueue(this->GetComponentId());
	this->SetPEventQueue(pPEventQueue);

	// Lifecycle Manager
	PLifecycleManager *pPLifecycleManager = new("PLifecycleManager") PLifecycleManager();
	this->SetPLifecycleManager(pPLifecycleManager);
	this->AllocateAComponent(pPLifecycleManager);
	// root event
	this->SendNoReplyEvent(this->GetUId(), (unsigned)IMain::EEventType::eInitializeAsAMain, 0, nullptr);

	LOG_FOOTER0();
}

void PMain::RunAsAMain() {
	Main::RunAsAMain();
}

void PMain::ShutdownSystem() {
	delete this->GetPEventQueue();
	delete this->GetPLifecycleManager();
}

void PMain::Initialize() {}
void PMain::Finalize() {}

///////////////////////////////////////////////
int PMain::main_ex() {
	PMemoryManager::Allocate(SIZE_MEMORY_SYSTEM, SIZE_MEMORY_APPLICATION, SIZE_PAGE, SIZE_SLOT_UNIT);

	PMain* pPMain = new("PMain") PMain();
	pPMain->BootstrapSystem();
	pPMain->RunAsAMain();
	pPMain->ShutdownSystem();
	delete pPMain;

	PMemoryManager::Delocate();
	return 0;
}
///////////////////////////////////////////////
