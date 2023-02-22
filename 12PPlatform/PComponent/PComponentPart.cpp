#include <12PPlatform/PComponent/PComponentPart.h>
#include <01Base/Aspect/Exception.h>

void* CallBackComponentPart(void *pObject) {
	PComponentPart *pPComponentPart = (PComponentPart *)pObject;
	pPComponentPart->RunThread();
	return nullptr;
}

void PComponentPart::Fork() {
	PThread::Fork(CallBackComponentPart, this);
}

void PComponentPart::Join() {
	PThread::Join();
}

PComponentPart::PComponentPart(int uClassId, const char* acClassName)
	: ComponentPart(uClassId, acClassName)
{
}

PComponentPart::~PComponentPart() {
}

void PComponentPart::RunThread() {
}

void PComponentPart::Start() {
	this->Fork();
}
void PComponentPart::Stop() {
	this->Join();
}