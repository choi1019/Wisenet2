#pragma once

#include <21Domain/VideoRequesterManager/IVideoRequesterManager.h>
#include <02Platform/Component/Component.h>
#include <01Base/Aspect/Log.h>

class VideoRequesterManager : public Component, public IVideoRequesterManager {
public:
	VideoRequesterManager(int nClassId = _VideoRequesterManager_Id,
		const char* pcClassName = _VideoRequesterManager_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~VideoRequesterManager() {
	}

protected:
	virtual void RegisterExceptions() {
	}
	
	void Initialize(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Initialize");
		LOG_FOOTER("VideoRequesterManage::Initialize");
	}
	void Start(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Start");
		LOG_FOOTER("VideoRequesterManage::Start");
	}
	void Process(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Process");
		LOG_FOOTER("VideoRequesterManage::Process");
	}
	void Stop(Event* pEvent) {
		LOG_HEADER("VideoRequesterManage::Stop");
		LOG_FOOTER("VideoRequesterManage::Stop");
	}

	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoRequesterManager::EEventType::eInitialize:
			this->Initialize(pEvent);
			break;
		case (unsigned)IVideoRequesterManager::EEventType::eStart:
			this->Start(pEvent);
			break;
		case (unsigned)IVideoRequesterManager::EEventType::eProcess:
			this->Process(pEvent);
			break;
		case (unsigned)IVideoRequesterManager::EEventType::eStop:
			this->Stop(pEvent);
			break;	
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
