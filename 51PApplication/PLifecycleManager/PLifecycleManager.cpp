#include "PLifecycleManager.h"

#include <12PPlatform/PComponent/PComponent.h>
#include <12PPlatform/PScheduler/PScheduler.h>
#include <13PTechnical/PTimer/PTimerRTC.h>
#include <13PTechnical/PTimer/PTimerLinux.h>
#include <13PTechnical/PTimer/PTimerLinux2.h>

PLifecycleManager::PLifecycleManager(
		unsigned typeId,
		const char* pClassName)
		: LifecycleManager(typeId, pClassName)
	{}
PLifecycleManager::~PLifecycleManager() {}

void PLifecycleManager::RegisterUserShedulers() {
	this->RegisterAScheduler((int)EComponents::eScheduler1, new("eScheduler1") PScheduler());
	//	this->RegisterAScheduler((int)EComponents::eScheduler2, new("eScheduler2") PScheduler());
}
void PLifecycleManager::RegisterUserComponents() {
	// this->RegisterAComponent((int)EComponents::eTimerLinux, new("eTimerLinux") PTimerLinux(900));
	this->RegisterAComponent((int)EComponents::eTimerLinux, new("eTimerLinux") PTimerLinux2(900));
	//	this->RegisterAComponent((int)EComponents::eTimerRTC, new("PTimerRTC") PTimerRTC(2000));
}
void PLifecycleManager::AllocateUserComponents() {
	this->AllocateAComponent((int)EComponents::eTimerLinux, (int)EComponents::eScheduler1);
	// this->AllocateAComponent((int)EComponents::eTimerRTC, (int)EComponents::eScheduler2);
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
	// BaseObject::s_pMemory->Show("Static");
	// ValueObject::s_pMemory->Show("Dynamic");
	this->SendReplyEvent((int)EComponents::eTimerLinux, (int)IComponent::EEventType::eStart);
	// this->SendReplyEvent((int)EComponents::eTimerRTC, (int)IComponent::EEventType::eStart);
}

void PLifecycleManager::StopComponents() {
	// this->SendReplyEvent((int)EComponents::eTimerLinux, (int)Component::EEventType::eStop);
}
