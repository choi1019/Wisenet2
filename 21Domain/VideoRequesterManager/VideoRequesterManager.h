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

	void Initialize() {
		Component::Initialize();
	}
	void Finalize() {
		Component::Finalize();
	}

	void Start(Event* pEvent) override {
	}
	
private:
	void Register(Event* pEvent) {
		if (pEvent->IsReply()) {
			LOG_FOOTER("VideoRequesterManage::Register");
		}
		else {
			LOG_HEADER("VideoRequesterManage::Register");
			this->SendReplyEvent(this->GetUId(), (int)IComponent::EEventType::eStart);
		}
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoRequesterManager::EEventType::eRegister:
			this->Register(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
