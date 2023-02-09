#include <12PPlatform/PComponent/PComponentPart.h>

PComponentPart::PComponentPart(unsigned uClassId, const char* acClassName)
	: ComponentPart(uClassId, acClassName)
{
	this->RegisterExceptions();

}

PComponentPart::~PComponentPart() {
}

void PComponentPart::RegisterExceptions() {
	ComponentPart::RegisterExceptions();
}

void PComponentPart::Initialize() {
	ComponentPart::Initialize();
}
void PComponentPart::Finalize() {
	ComponentPart::Finalize();
}

void PComponentPart::Start() {
}

void PComponentPart::Pause() {
}

void PComponentPart::Stop() {
}

void PComponentPart::Run() {

}