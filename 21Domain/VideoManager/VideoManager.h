#pragma once

#include <02Platform/Component/Component.h>

// provided interfacec
#include <21Domain/VideoManager/IVideoManager.h>

// required interface
#include <03Technical/Timer/ITimer.h>
#include <21Domain/VideoProviderManager/IVideoProviderManager.h>

class VideoManager : public IVideoManager, public Component {

private:
	int m_nCount;

public:
	VideoManager(
		int nClassId = _VideoManager_Id,
		const char* pcClassName = _VideoManager_Name)
		: Component(nClassId, pcClassName)
		, m_nCount(0)
	{}
	virtual ~VideoManager() {}

protected:
	virtual void RegisterEventTypes() {
		Component::RegisterEventTypes();
	}

	virtual void RegisterExceptions() {
		Component::RegisterExceptions();
	}

private:
	void Start(Event* pEvent) {
		if (pEvent->IsReply()) {
			LOG_FOOTER("VideoManager::Start");
		}
		else {
			LOG_HEADER("VideoManager::Start");
			Component::Start(pEvent);
			this->SendReplyEvent(this->GetUId(), (int)IVideoManager::EEventType::eRegister);
		}
	}

	void Register(Event* pEvent) {
		if (pEvent->IsReply()) {
			LOG_FOOTER("VideoManager::Register");
		}
		else {
			LOG_HEADER("VideoManager::Register");
			Component::Stop(pEvent);
			this->SendReplyEvent(
				(int)IVideoManager::EReceivers::eVideoProviderManager, 
				(int)IVideoProviderManager::EEventType::eRegister);
		}
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (int)IVideoManager::EEventType::eRegister:
			this->Register(pEvent);
			//this->Stop(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
