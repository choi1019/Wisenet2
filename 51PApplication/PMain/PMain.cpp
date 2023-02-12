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
	LOG_HEADER(this->GetClassName(), "BootstrapSystem");

	// Main EventQueue
	PEventQueue* pPEventQueue = new("PMain::PEventQueue") PEventQueue(this->GetComponentId());
	this->SetPEventQueue(pPEventQueue);

	// Lifecycle Manager
	PLifecycleManager *pPLifecycleManager = new("PLifecycleManager") PLifecycleManager();
	this->SetPLifecycleManager(pPLifecycleManager);
	this->AllocateAComponent(pPLifecycleManager);

	LOG_FOOTER(this->GetClassName(), "BootstrapSystem");
	
	// send system start event
	this->SendStartEvent();
}

void PMain::ShutdownSystem() {
	delete this->GetPEventQueue();
	delete this->GetPLifecycleManager();
}

void PMain::Initialize() {}
void PMain::Finalize() {}

int PMain::main_ex() {
	size_t m_szSystemMemory;
	char* m_pSystemMemeoryAllocated;
	PMemoryStatic* m_pMemoryStatic;
	size_t m_szApplicationMemory;
	char* m_pApplicationMemeoryAllocated;
	PMemoryDynamic* m_pMemoryDynamic;

	// system memory allocation
	m_szSystemMemory = SIZE_MEMORY_SYSTEM;
	m_pSystemMemeoryAllocated = new char[m_szSystemMemory];
	m_pMemoryStatic = new(m_pSystemMemeoryAllocated, m_szSystemMemory, "main_ex") PMemoryStatic();
	m_pMemoryStatic->Initialize();
	m_pMemoryStatic->Show("m_pMemoryStatic::Initialize()");

	// aplication memorty allocation
	m_szApplicationMemory = SIZE_MEMORY_APPLICATION;
	m_pApplicationMemeoryAllocated = new char[m_szApplicationMemory];
	m_pMemoryDynamic = new(m_pApplicationMemeoryAllocated, m_szApplicationMemory, "main_ex") PMemoryDynamic(SIZE_PAGE, SIZE_SLOT_UNIT);
	m_pMemoryDynamic->Initialize();
	m_pMemoryDynamic->Show("m_pMemoryDynamic::Initialize()");

	///////////////////////////////////////////////
	PMain* pPMain = new("PMain") PMain();
	pPMain->BootstrapSystem();
	pPMain->Run();
	pPMain->ShutdownSystem();
	///////////////////////////////////////////////

	m_pMemoryDynamic->Finalize();
	m_pMemoryDynamic->Show("");
	delete m_pMemoryDynamic;
	delete[] m_pApplicationMemeoryAllocated;

	m_pMemoryStatic->Finalize();
	m_pMemoryStatic->Show("");
	delete m_pMemoryStatic;
	delete[] m_pSystemMemeoryAllocated;

	return 0;
}
