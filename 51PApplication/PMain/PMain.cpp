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
	// system memory allocation
	size_t m_szSystemMemory = SIZE_MEMORY_SYSTEM;
	char* m_pSystemMemeoryAllocated = new char[m_szSystemMemory];
	PMemoryStatic* m_pMemoryStatic = new(m_pSystemMemeoryAllocated, m_szSystemMemory) PMemoryStatic();
	m_pMemoryStatic->Initialize();
	m_pMemoryStatic->Show("m_pMemoryStatic::Initialize()");

	// aplication memorty allocation
	size_t m_szUserMemory = SIZE_MEMORY_APPLICATION;
	char* m_pUserMemeoryAllocated = new char[m_szUserMemory];
	PMemoryDynamic* m_pMemoryDynamic = new(m_pUserMemeoryAllocated, m_szUserMemory) PMemoryDynamic(SIZE_PAGE, SIZE_SLOT_UNIT);
	m_pMemoryDynamic->Initialize();
	m_pMemoryDynamic->Show("m_pMemoryDynamic::Initialize()");

	///////////////////////////////////////////
	PMain* pPMain = new("PMain") PMain();

	pPMain->BootstrapSystem();
	pPMain->Run();
	pPMain->ShutdownSystem();
	///////////////////////////////////////////

	m_pMemoryDynamic->Finalize();
	m_pMemoryDynamic->Show("");
	delete m_pMemoryDynamic;
	delete[] m_pUserMemeoryAllocated;

	m_pMemoryStatic->Finalize();
	m_pMemoryStatic->Show("");
	delete m_pMemoryStatic;
	delete[] m_pSystemMemeoryAllocated;
	
	return 0;
}
