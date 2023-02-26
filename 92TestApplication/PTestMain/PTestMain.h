#pragma once

#include <92TestApplication/PTestMain/typedef.h>
#define _PTestMain_ID _GET_TESTCLASS_UID(_ETestComponent_PMain::_ePTestMain)
#define _PTestMain_NAME "PTestMain"

#include <91TestPlatform/TestCase/TestMain.h>
#include <92TestApplication/PTS1/PTS1.h>

class PTestMain : public TestMain {
public:
	PTestMain(unsigned nClassId = _PTestMain_ID, const char* pcClassName = _PTestMain_NAME)
	: TestMain(nClassId, pcClassName)
	{}
	~PTestMain() override {
	}
	void Initialize() override {
		TestMain::Initialize();
		this->Add(new("PTS1") PTS1());
	}

	void Finalize() override {
		TestMain::Finalize();
	}
};
