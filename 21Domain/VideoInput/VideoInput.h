#pragma once

#include <21Domain/VideoInput/IVideoInput.h>
#include <02Platform/Component/Component.h>
#include <01Base/Aspect/Log.h>

class VideoInput : public Component {
public:
	VideoInput(int nClassId = _VideoInput_Id,
		const char* pcClassName = _VideoInput_Name)
		: Component(nClassId, pcClassName)
	{
		this->RegisterExceptions();
	}
	virtual ~VideoInput() {
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
	void ParseFrame(Event* pEvent) {
		LOG(this->GetClassName(), __func__, "");

		if (pEvent->IsReply()) {
			LOG_FOOTER("ProcessAFrame eParseFrame");
		}
		else {
			LOG_HEADER("ProcessAFrame eParseFrame");

		}
	}

protected:
	void ProcessAEvent(Event* pEvent) {
		switch (pEvent->GetType()) {
		case (unsigned)IVideoInput::EEventType::eParseFrame:
		// {
		// 	this->SetTargetComponentId((unsigned int)IPLifecycleManager::EComponents::eVideoInput);
		// 	this->SendStream(pEvent);
		// 	if (pEvent->GetESyncType() == Event::ESyncType::eSync) {
		// 		this->ReceiveStream();
		// 		IVideoInput::Result* pResult = new("") IVideoInput::Result((IVideoInput::Result&)*this->GetReceivedPArg());
		// 		delete pEvent->GetPArg();
		// 		pEvent->SetlArg(this->GetReceivedEvent()->GetlArg());
		// 		pEvent->SetPArg(pResult);
		// 	}
		// 	break;
		// }
		default:
			Component::ProcessAEvent(pEvent);
			break;
		}
	}
};
