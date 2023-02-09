#include <12PPlatform/PComponent/PComponent.h>
#include <01Base/Aspect/Exception.h>

void* CallBackPComponent(void *pObject) {
	PComponent *pPComponent = (PComponent *)pObject;
	pPComponent->Run();
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
	m_idThared = pthread_create((&m_thread), NULL, CallBackPComponent, (void*)this);
	if(m_idThared < 0) {
		throw Exception();
	}
}
void PComponent::Stop() {
	pthread_join(m_thread, (void **)&m_stsThread);
}