#pragma once

#include <92TestApplication/typedef.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS2)
#define _PTS2_Name "PTS2"
#include <91TestPlatform/TestCase/TestSuite.h>

#include <92TestApplication/PTS2/Config.h>
//#include <13PTechnical/PMemoryManager/PMemoryManager.h>
#include <19PApplication/PMain/PMain.h>

class PTS2: public TestSuite {
public:
	PTS2(
		unsigned classId = _PTS2_Id,
		const char* pClassName = _PTS2_Name)
		: TestSuite(classId, pClassName)
	{
	}
	virtual ~PTS2() {
	}

	void Initialize() {
		TestSuite::Initialize();
		//PMemoryManager::Allocate(SIZE_MEMORY_SYSTEM, SIZE_MEMORY_APPLICATION, SIZE_PAGE, SIZE_SLOT_UNIT);
	}

	void Run() override {
		PMain::main_ex();
	}

	void Finalize() {
		TestSuite::Finalize();
		//PMemoryManager::Delocate();
	}
};

