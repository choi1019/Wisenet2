#pragma once

#include <02Platform/Component/Component.h>

// provided interfacec
#include <21Domain/VideoManager/IVideoManager.h>

// required interface
#include <03Technical/Timer/ITimer.h>
#include <21Domain/VideoInput/IVideoInput.h>

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

	void Initialize() {
		Component::Initialize();
	}
	void Finalize() {
		Component::Finalize();
	}

	void Stop() {
		Component::Stop();

	}

private:
	void Start(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)Component::EEventType::eStart) {
				if (pEvent->GetUIdSource().GetComponentId() == this->FindUid((int)IVideoManager::EReceivers::eVideoInput).GetComponentId()) {
					this->SendReplyEvent((int)IVideoManager::ESources::eTimer, (int)Component::EEventType::eStart);
				}
			}
		}
		else {
			Component::Start(pEvent);
			this->SendReplyEvent((int)IVideoManager::EReceivers::eVideoInput, (int)Component::EEventType::eStart);
		}
	}

	void Stop(Event* pEvent) {
		if (pEvent->IsReply()) {
		}
		else {
			Component::Stop(pEvent);
			this->SendReplyEvent((int)IVideoManager::ESources::eTimer, (int)Component::EEventType::eStop);
		}
	}

	void Run(Event* pEvent) {
		if (pEvent->IsReply()) {
			if (pEvent->GetReplyType() == (int)IVideoInput::EEventType::eParseFrame) {
				LOG(this->GetClassName(), __func__, "Reply eParseFrame");
			}
		}
		else {
			IVideoInput::ParamCompute* pParamCompute = new("ParamCompute") IVideoInput::ParamCompute(1, "eParseFrame");
			this->SendReplyEvent((int)IVideoManager::EReceivers::eVideoInput, (int)IVideoInput::EEventType::eParseFrame, 0, pParamCompute);
		}
	}

	void Pause(Event* pEvent) {
	}

protected:
	// void ProcessAEvent(Event* pEvent) {
	// 	switch (pEvent->GetType()) {
	// 	case (int)ITimer::EEventType::eTimeOut:
	// 		this->Run(pEvent);
	// 		//this->Stop(pEvent);
	// 		break;
	// 	default:
	// 		Component::ProcessAEvent(pEvent);
	// 		break;
	// 	}
	// }
};
