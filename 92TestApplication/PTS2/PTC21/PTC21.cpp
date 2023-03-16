
#include <92TestApplication/PTS2/PTC21/PTC21.h>
#include <92TestApplication/PTS2/PTC21/Config.h>

#include <19PApplication/PMain/PMain.h>

PTC21::PTC21(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
{
}
PTC21::~PTC21() {
}

void PTC21::Initialize() {
}
void PTC21::Finalize() {
}
void PTC21::Run() {
    PMain::main_ex();
}