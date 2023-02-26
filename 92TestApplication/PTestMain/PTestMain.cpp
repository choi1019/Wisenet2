
#include <92TestApplication/PTestMain/PTestMain.h>
#include <92TestApplication/PTestMain/Config.h>

#include <92TestApplication/PTS1/PTS1.h>

PTestMain::PTestMain(unsigned nClassId, const char* pcClassName)
	: TestMain(nClassId, pcClassName)
{
}
PTestMain::~PTestMain() {
}

void PTestMain::Initialize() {
	this->Add(new("PTS1") PTS1());
}

void PTestMain::Finalize() {
}
