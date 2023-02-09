#include <12PPlatform/PComponent/PComponent.h>

PComponent::PComponent(
	int nClassId, const char* pcClassName)
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
}

void PComponent::Pause() {
}

void PComponent::Stop() {

}

void PComponent::Run() {
}