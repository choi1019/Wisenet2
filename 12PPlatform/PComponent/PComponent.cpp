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
	int result = pthread_mutex_init(&m_mutex, nullptr);
    if (result != 0) {
        throw Exception(-1, "PEventQueue::PEventQueue");
    }
}
PComponent::~PComponent() {
}

void PComponent::RegisterEventTypes() {
	Component::RegisterEventTypes();
}
void PComponent::RegisterExceptions() {
	Component::RegisterExceptions();
}

IComponent::EState PComponent::GetEState() {
	pthread_mutex_lock(&m_mutex);
	IComponent::EState eState = this->m_eState;
	pthread_mutex_unlock(&m_mutex); 
	return eState; 
}
void PComponent::SetEState(IComponent::EState eState) { 
	pthread_mutex_lock(&m_mutex);
	this->m_eState = eState;
	pthread_mutex_unlock(&m_mutex); 
}

void PComponent::Fork() {
	PThread::Fork(CallBackPComponent, this);
}

void PComponent::Join() {
	PThread::Join();
}

void PComponent::Start() {
	this->SetEState(IComponent::EState::eRunning);
	this->Fork();
}
void PComponent::Stop() {
	this->SetEState(IComponent::EState::eStopped); 
	this->Join();
}

