#include <12PPlatform/PComponent/PComponentPart.h>

#include <01Base/Aspect/Exception.h>

void* CallBackPComponentPart(void *pObject) {
	PComponentPart *pPComponentPart = (PComponentPart *)pObject;
	pPComponentPart->Run();
	return nullptr;
}

PComponentPart::PComponentPart(int uClassId, const char* acClassName)
	: ComponentPart(uClassId, acClassName)
{
}

PComponentPart::~PComponentPart() {
}

void PComponentPart::Start() {
	this->SetEState(IComponent::EState::eRunning);
	m_idThared = pthread_create((&m_thread), NULL, CallBackPComponentPart, (void*)this);
	if(m_idThared < 0) {
		throw Exception();
	}

}
void PComponentPart::Stop() {
	this->SetEState(IComponent::EState::eStopped);
	pthread_join(m_thread, (void**)&m_stsThread);
}