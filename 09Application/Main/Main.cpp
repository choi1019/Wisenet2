#include <09Application/Main/Main.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

//----------------------------------------------------------------------------
void Main::RegisterEventTypes() {
	Directory::s_dirEvents[(int)IMain::EEventType::eInitializeAsAMain] = "eInitializeAsAMain";
	Directory::s_dirEvents[(int)IMain::EEventType::eRunAsAMain] = "eRunAsAMain";
	Directory::s_dirEvents[(int)IMain::EEventType::eFinalizeAsAMain] = "eFinalizeAsAMain";
}

void  Main::RegisterExceptions() {
//	REGISTER_EVENT(IMain, eEventTypeError);
}
//----------------------------------------------------------------------------

Main::Main(
	EventQueue *pEventQueue,
	unsigned uClassId,
	const char* pcClassName)
	: Scheduler(pEventQueue, uClassId, pcClassName)
	, m_pLifecycleManager(nullptr)
{
	this->RegisterEventTypes();
	this->RegisterExceptions();
}
Main::~Main() {
	Directory::s_dirComponents.Clear();
	Directory::s_dirEvents.Clear();
	Directory::s_dirExceptions.Clear();
}

void Main::RunAsAMain() {
	Scheduler::RunAsAScheduler();
}

void Main::InitializeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		if (pEvent->GetReplyType() == (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager) {
			LOG_FOOTER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eInitializeAsALifecycleManager)");
			this->SendReplyEvent(this->m_pLifecycleManager->GetUId(), (int)ILifecycleManager::EEventType::eStartSystem);
			LOG_HEADER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eStartSystem)");
		} else if (pEvent->GetReplyType() == (int)ILifecycleManager::EEventType::eStartSystem) {
			this->SendNoReplyEvent(this->GetUId(), (int)IMain::EEventType::eFinalizeAsAMain);
			LOG_FOOTER("Main::InitializeAsAMain", "Send->Main(eFinalizeAsAMain)");
		} else {
			throw Exception ((unsigned)IComponent::EException::eEventNotSupported, "Main", "InitializeAsAMain", pEvent->GetType());
		}
	}
	else {
		LOG_HEADER("Main::InitializeAsAMain", "Send->m_pLifecycleManager(eInitializeAsALifecycleManager)");
		// send a trigger event
		ILifecycleManager::ParamInitializeAsALifecycleManager* pParamParamInitializeAsALifecycleManager
			= new("ParamInitializeAsALifecycleManager") ILifecycleManager::ParamInitializeAsALifecycleManager(this);
		this->SendReplyEvent(
			this->m_pLifecycleManager->GetUId()
			, (int)ILifecycleManager::EEventType::eInitializeAsALifecycleManager
			, UNDEFINED
			, pParamParamInitializeAsALifecycleManager
		);
	}

}

void Main::FinalizeAsAMain(Event* pEvent) {
	if (pEvent->IsReply()) {
		Scheduler::StopAsAScheduler();
		LOG_FOOTER("Main::FinalizeAsAMain");
	}
	else {
		LOG_HEADER("Main::FinalizeAsAMain");
		this->SendReplyEvent(this->m_pLifecycleManager->GetUId(),
			(unsigned)ILifecycleManager::EEventType::eFinalizeAsALifecycleManager);
	}
}

void Main::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (unsigned)IMain::EEventType::eInitializeAsAMain:
		this->InitializeAsAMain(pEvent);
		break;
	case (unsigned)IMain::EEventType::eFinalizeAsAMain:
		this->FinalizeAsAMain(pEvent);
		break;
	default:
		Scheduler::ProcessAEvent(pEvent);
		break;
	}
}

