#include "PLifecycleManager.h"

#include <12PPlatform/PComponent/PComponent.h>
#include <12PPlatform/PScheduler/PScheduler.h>
#include <13PTechnical/PTimer/PTimerRTC.h>
#include <13PTechnical/PTimer/PTimerLinux.h>

PLifecycleManager::PLifecycleManager(
		unsigned typeId,
		const char* pClassName)
		: LifecycleManager(typeId, pClassName)
	{}
PLifecycleManager::~PLifecycleManager() {}

void PLifecycleManager::RegisterUserShedulers() {
	this->RegisterAScheduler((int)EComponents::eScheduler1, new("eScheduler1") PScheduler());
	this->RegisterAScheduler((int)EComponents::eScheduler2, new("eScheduler2") PScheduler());
	// this->RegisterAScheduler((int)EComponents::eVideoInput, new("eVideoInput") VideoInput());
}
void PLifecycleManager::RegisterUserComponents() {
	this->RegisterAComponent((int)EComponents::eTimer1, new("eTimerLinux1") PTimerLinux(1));
	this->RegisterAComponent((int)EComponents::eTimer2, new("eTimerLinux2") PTimerLinux(1));
	// this->RegisterAComponent((int)EComponents::eVideoManager, new("VideoManager") VideoManager());
}
void PLifecycleManager::AllocateUserComponents() {
	this->AllocateAComponent((int)EComponents::eTimer1, (int)EComponents::eScheduler1);
	this->AllocateAComponent((int)EComponents::eTimer2, (int)EComponents::eScheduler2);
	// this->AllocateAComponent((int)EComponents::eVideoManager, (int)EComponents::eScheduler1);
}
void PLifecycleManager::AssociateUserSendersNReceivers() {
	// this->AssociateASenderNAReceiver(
	// 	(int)EComponents::eVideoManager, (int)IVideoManager::ESources::eTimer, (int)EComponents::eTimer33);
	// this->AssociateASenderNAReceiver(
	// 	(int)EComponents::eVideoManager, (int)IVideoManager::EReceivers::eVideoInput, (int)EComponents::eVideoInput);
}
void PLifecycleManager::AssociateUserSourcesNTargets() {
	// this->AssociateASourceNATarget((int)EComponents::eTimer33, (int)Timer::EGroups::eGroup1, (int)EComponents::eVideoManager);
}

void PLifecycleManager::Initialize() {
	LifecycleManager::Initialize();
}
void PLifecycleManager::Finalize() {
	LifecycleManager::Finalize();
}

void PLifecycleManager::StartComponents() {
//	this->SendReplyEvent((int)EComponents::eVideoManager, (int)Component::EEventType::eStart);
	BaseObject::s_pMemory->Show("Static");
	ValueObject::s_pMemory->Show("Dynamic");
	this->m_mapComponents[(int)EComponents::eTimer1]->Start();
	this->m_mapComponents[(int)EComponents::eTimer2]->Start();;
}

void PLifecycleManager::StopComponents() {
//	this->SendReplyEvent((int)EComponents::eVideoManager, (int)Component::EEventType::eStop);
}
