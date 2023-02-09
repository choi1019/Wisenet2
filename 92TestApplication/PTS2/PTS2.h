#pragma once

#include <92TestApplication/typedef.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS2)
#define _PTS2_Name "PTS2"
#include <92TestApplication/PTS2/Config.h>

#include <13PTechnical/PMemoryManager/PMemoryStatic.h>
#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

#include <91TestPlatform/TestCase/TestSuite.h>
// #include <92TestApplication/PTS2/PTC21/PTC21.h>
// #include <92TestApplication/PTS2/PTC22/PTC22.h>

#include <51PApplication/PMain/PMain.h>

class PTS2: public TestSuite {
private:
	size_t m_szSystemMemory;
	char* m_pSystemMemeoryAllocated;
	PMemoryStatic* m_pMemoryStatic;
	size_t m_szApplicationMemory;
	char* m_pApplicationMemeoryAllocated;
	PMemoryDynamic* m_pMemoryDynamic;

public:
	PTS2(
		unsigned classId = _PTS2_Id,
		const char* pClassName = _PTS2_Name)
		: TestSuite(classId, pClassName)
	{
	}
	virtual ~PTS2() override {
	}

	void Initialize() override {
		// system memory allocation
		m_szSystemMemory = SIZE_MEMORY_SYSTEM;
		m_pSystemMemeoryAllocated = new char[m_szSystemMemory];
		m_pMemoryStatic = new(m_pSystemMemeoryAllocated, m_szSystemMemory) PMemoryStatic();
		m_pMemoryStatic->Initialize();
		m_pMemoryStatic->Show("m_pMemoryStatic::Initialize()");

		// aplication memorty allocation
		m_szApplicationMemory = SIZE_MEMORY_APPLICATION;
		m_pApplicationMemeoryAllocated = new char[m_szApplicationMemory];
		m_pMemoryDynamic = new(m_pApplicationMemeoryAllocated, m_szApplicationMemory) PMemoryDynamic(SIZE_PAGE, SIZE_SLOT_UNIT);
		m_pMemoryDynamic->Initialize();
		m_pMemoryDynamic->Show("m_pMemoryDynamic::Initialize()");
	}
	
	void Run() override {
		PMain* pPMain = new("PMain") PMain();
		pPMain->BootstrapSystem();
		pPMain->Run();
		pPMain->ShutdownSystem();
	}

	void Finalize() override {
		m_pMemoryDynamic->Finalize();
		m_pMemoryDynamic->Show("");
		delete m_pMemoryDynamic;
		delete[] m_pApplicationMemeoryAllocated;

		m_pMemoryStatic->Finalize();
		m_pMemoryStatic->Show("");
		delete m_pMemoryStatic;
		delete[] m_pSystemMemeoryAllocated;
	}
};
