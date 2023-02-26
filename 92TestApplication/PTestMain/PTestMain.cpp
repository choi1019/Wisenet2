
#include <92TestApplication/PTestMain/PTestMain.h>
#include <92TestApplication/PTestMain/Config.h>

#include <92TestApplication/PTS1/PTS1.h>
#include <92TestApplication/PTS2/PTS2.h>

PTestMain::PTestMain(unsigned nClassId, const char* pcClassName)
	: TestMain(nClassId, pcClassName)
{
}
PTestMain::~PTestMain() {
}

void PTestMain::Initialize() {
	this->Add(new("PTS1") PTS1());
	this->Add(new("PTS2") PTS2());
}

void PTestMain::Finalize() {
}
