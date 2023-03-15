#include <12PPlatform/PComponent/PComponentPart.h>
#include <01Base/Aspect/Exception.h>

void* CallBackComponentPart(void *pObject) {
	PComponentPart *pPComponentPart = (PComponentPart *)pObject;
	pPComponentPart->RunThread();
	return nullptr;
}

PComponentPart::PComponentPart(int uClassId, const char* acClassName)
	: ComponentPart(uClassId, acClassName)
{
	int result = pthread_mutex_init(&m_mutex, nullptr);
    if (result != 0) {
        throw Exception(-1, "PEventQueue::PEventQueue");
    }
}

PComponentPart::~PComponentPart() {
}

IComponent::EState PComponentPart::GetEState() {
	pthread_mutex_lock(&m_mutex);
	IComponent::EState eState = this->m_eState;
	pthread_mutex_unlock(&m_mutex); 
	return eState; 
}
void PComponentPart::SetEState(IComponent::EState eState) { 
	pthread_mutex_lock(&m_mutex);
	this->m_eState = eState;
	pthread_mutex_unlock(&m_mutex); 
}

void PComponentPart::Fork() {
	PThread::Fork(CallBackComponentPart, this);
}

void PComponentPart::Join() {
	PThread::Join();
}

void PComponentPart::Start() {
	this->SetEState(IComponent::EState::eRunning);
	this->Fork();
}
void PComponentPart::Stop() {
	this->SetEState(IComponent::EState::eStopped); 
	this->Join();
}