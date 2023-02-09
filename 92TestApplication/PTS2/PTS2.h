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
	}
	
	void Run() override {
		PMain::main_ex();
	}

	void Finalize() override {
	}
};
