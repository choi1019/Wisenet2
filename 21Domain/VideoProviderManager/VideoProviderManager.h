#pragma once

#include <21Domain/VideoProviderManager/IVideoProviderManager.h>
#include <02Platform/Component/Component.h>
#include <01Base/Aspect/Log.h>

#include <21Domain/VideoRequesterManager/VideoRequesterManager.h>

class VideoProviderManager : public Component, public IVideoProviderManager {
public:
	VideoProviderManager(int nClassId = _VideoProviderManager_Id,
		const char* pcClassName = _VideoProviderManager_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~VideoProviderManager() {
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

private:
	void Register(Event* pEvent) {
		if (pEvent->IsReply()) {
			LOG_FOOTER("VideoProviderManager::Register");
		}
		else {
			LOG_HEADER("VideoProviderManager::Register");
			this->SendReplyEvent(
				(int)IVideoProviderManager::EReceivers::eVideoRequesterManager, 
				(int)IVideoRequesterManager::EEventType::eRegister);
		}
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoProviderManager::EEventType::eRegister:
			this->Register(pEvent);
			break;
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
