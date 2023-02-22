#include <12PPlatform/PComponent/PComponent.h>
#include <01Base/Aspect/Exception.h>

void* CallBackPComponent(void *pObject) {
	PComponent *pPComponent = (PComponent *)pObject;
	pPComponent->RunThread();
	return nullptr;
}

PComponent::PComponent(int nClassId, const char* pcClassName)
	: Component(nClassId, pcClassName)	
{
}
PComponent::~PComponent() {
}

void PComponent::RegisterEventTypes() {
	Component::RegisterEventTypes();
}
void PComponent::RegisterExceptions() {
	Component::RegisterExceptions();
}

void PComponent::Initialize() {
	Component::Initialize();
}
void PComponent::Finalize() {
	Component::Finalize();
}

void PComponent::Start() {
	this->Fork();
}
void PComponent::Stop() {
	this->Join();
}

void PComponent::Fork() {
	PThread::Fork(CallBackPComponent, this);
}

void PComponent::Join() {
	PThread::Join();
}