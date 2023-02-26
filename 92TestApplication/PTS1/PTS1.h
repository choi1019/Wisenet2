#pragma once

#include <92TestApplication/typedef.h>
#define _PTS1_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS1)
#define _PTS1_Name "PTS1"
#include <91TestPlatform/TestCase/TestSuite.h>


#include <92TestApplication/PTS1/Config.h>
#include <13PTechnical/PMemoryManager/PMemoryManager.h>
#include <12PPlatform/PScheduler/PScheduler.h>

#include <92TestApplication/PTS1/PTC11/PTC11.h>
#include <92TestApplication/PTS1/PTC12/PTC12.h>
#include <92TestApplication/PTS1/PTC13/PTC13.h>
#include <92TestApplication/PTS1/PTC14/PTC14.h>

class PTS1: public TestSuite {
public:
	PTS1(
		unsigned classId = _PTS1_Id,
		const char* pClassName = _PTS1_Name)
		: TestSuite(classId, pClassName)
	{
	}
	virtual ~PTS1() {
	}

	void Initialize() {
		TestSuite::Initialize();
		try {
    		PMemoryManager::Allocate(SIZE_MEMORY_SYSTEM, SIZE_MEMORY_APPLICATION, SIZE_PAGE, SIZE_SLOT_UNIT);
			this->Add(new("PTC11") PTC11());
			this->Add(new("PTC12") PTC12());
			this->Add(new("PTC13") PTC13());
			this->Add(new("PTC14") PTC14());
		}
		catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}

	void Run() override {
		TestSuite::Run();
		// this->InitializeAsAScheduler(0);
		// this->Fork();

	}

	void Finalize() {
		TestSuite::Finalize();
		try {
    		PMemoryManager::Delocate();
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}
};

