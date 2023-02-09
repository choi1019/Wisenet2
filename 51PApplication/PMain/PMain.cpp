#include <51PApplication/PMain/PMain.h>

#include <51PApplication/PMain/Config.h>
#include <13PTechnical/PMemoryManager/PMemoryStatic.h>
#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <51PApplication/PLifecycleManager/PLifecycleManager.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>

PMain::PMain(unsigned uClassId,
	const char* pcClassName)
	: Main(uClassId, pcClassName)
{
}
PMain::~PMain() {
}

void PMain::BootstrapSystem() {
	LOG_HEADER(this->GetClassName(), __func__);

	// Main EventQueue
	PEventQueue* pPEventQueue = new("PMain::PEventQueue") PEventQueue(this->GetComponentId());
	LOG_NEWLINE("new", Directory::s_dirObjects[(long long)pPEventQueue]);
	this->SetPEventQueue(pPEventQueue);

	// Lifecycle Manager
	this->SetPLifecycleManager(new("PLifecycleManager") PLifecycleManager());
	this->AllocateAComponent(this->GetPLifecycleManager());

	LOG_FOOTER(this->GetClassName(), __func__);

	this->SendStartEvent();
}

void PMain::ShutdownSystem() {
	delete this->GetPEventQueue();
	delete this->GetPLifecycleManager();
}

int PMain::main_ex() {
	///////////////////////////////////////////

	///////////////////////////////////////////
	return 0;
}
