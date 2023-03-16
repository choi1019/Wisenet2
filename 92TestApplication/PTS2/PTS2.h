#pragma once

#include <92TestApplication/typedef.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS2)
#define _PTS2_Name "PTS2"
#include <91TestPlatform/TestCase/TestSuite.h>

#include <92TestApplication/PTS2/Config.h>
#include <92TestApplication/PTS2/PTC21/PTC21.h>
#include <92TestApplication/PTS2/PTC22/PTC22.h>

class PTS2: public TestSuite {
public:
	PTS2(
		unsigned classId = _PTS2_Id,
		const char* pClassName = _PTS2_Name)
		: TestSuite(classId, pClassName) {
		this->Add(new("PTC21") PTC21());
		this->Add(new("PTC22") PTC22());
	}
	
	virtual ~PTS2() {
	}
};

