
#include <02Platform/Scheduler/Scheduler.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

void Scheduler::InitializeVarialbes() {
	this->m_mComponents.Clear();
	//		this->m_pEventQueue->Clear();
}
void Scheduler::DeleteVarialbes() {
	LOG_HEADER("Scheduler::DeleteVarialbes", Directory::s_dirComponents[this->GetComponentId()], "EventQueue");
	this->m_pEventQueue->Show(Directory::s_dirComponents[this->GetComponentId()].c_str());

//	this->GetPEventQueue()->ShowState("");
	Event* pEvent = this->m_pEventQueue->Front();
	while (pEvent != nullptr) {
		LOG_NEWLINE(__func__, Directory::s_dirEvents[pEvent->GetType()]);
		Event *pPoppedEvent = this->m_pEventQueue->PopFront();
		if (pPoppedEvent != nullptr) {
			delete pEvent;
		}
		pEvent = this->m_pEventQueue->Front();
	}
	LOG_FOOTER("Scheduler::DeleteVarialbes", Directory::s_dirComponents[this->GetComponentId()], "EventQueue");
}

void Scheduler::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eInitializeAsAScheduler] = "eInitializeAsAScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eAllocateAComponent] = "eAllocateAComponent";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eDellocateAComponent] = "eDellocateAComponent";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eFinalizeAsAScheduler] = "eFinalizeAsAScheduler";

	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eFork] = "eFork";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eJoin] = "eJoin";

	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eStart] = "eStartScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eStop] = "eStopScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::ePause] = "ePauseScheduler";
	Directory::s_dirEvents[(unsigned)IScheduler::EEventType::eResume] = "eResumeScheduler";
}

Scheduler::Scheduler(
	unsigned classId, const char* className)
	: Component(classId, className)
	//		, m_nSchedulerId(s_uCounter++)
	, m_pEventQueue(nullptr)
{
	this->RegisterEventTypes();
	this->m_eState = IScheduler::EState::eCreated;
}
Scheduler::~Scheduler() {
}

void Scheduler::Initialize() {
	Component::Initialize();
}
void Scheduler::Finalize() {
	Component::Finalize();
}


//	int GetSchedulerId() { return this->m_nSchedulerId; }
EventQueue* Scheduler::GetPEventQueue() { 
	return this->m_pEventQueue; 
}

void Scheduler::SetPEventQueue(EventQueue* pEventQueue) {
	this->m_pEventQueue = pEventQueue;
	this->AllocateAComponent(this);
}

// jobs to do before the thread is started
void Scheduler::InitializeAsAScheduler(int uPriority) {
	this->InitializeVarialbes();
	this->m_eState = IScheduler::EState::eInitializedAsAScheduler;
}
void Scheduler::FinalizeAsAScheduler() {
	this->m_eState = IScheduler::EState::eFinalizedAsAScheduler;
	this->DeleteVarialbes();
}

void Scheduler::PauseAsAScheduler() {
	m_eState = IScheduler::EState::ePaused;
}
void Scheduler::ResumeAsAScheduler() {
	m_eState = IScheduler::EState::eStarted;
}
void Scheduler::StartAsAScheduler() {
	m_eState = IScheduler::EState::eStarted;
}
void Scheduler::StopAsAScheduler() {
	m_eState = IScheduler::EState::eStopped;
}

void Scheduler::Fork(Event* pEvent) {
	this->Fork();
}
void Scheduler::Join(Event* pEvent) {
	this->Join();
}
void Scheduler::StartAsAScheduler(Event* pEvent) {
	this->StartAsAScheduler();
}
void Scheduler::StopAsAScheduler(Event* pEvent) {
	this->StopAsAScheduler();
}
void Scheduler::PauseAsAScheduler(Event* pEvent) {
	this->PauseAsAScheduler();
}
void Scheduler::ResumeAsAScheduler(Event* pEvent) {
	this->ResumeAsAScheduler();
}


void Scheduler::RunAsAScheduler() {
	m_eState = IScheduler::EState::eRunning;
	LOG_NEWLINE(Directory::s_dirComponents[this->GetComponentId()], "RunAsAScheduler", "Start");
	while (IScheduler::EState::eStopped != this->m_eState)
	{
		RunOnce();
	}
	LOG_NEWLINE(Directory::s_dirComponents[this->GetComponentId()], "RunAsAScheduler", "End");
}

void Scheduler::RunOnce()
{
	try {
		// get pEvent from the queue
		Event* pEvent = pEvent = m_pEventQueue->PopFront();
		if (pEvent != nullptr) {
			UId uidTarget = pEvent->GetUIdTarget();
			Component* pTargetComponent = m_mComponents[uidTarget.GetComponentId()];
			if (pTargetComponent == nullptr) {
				throw Exception((unsigned)IScheduler::EError::eComponentNotFound, this->GetClassName(), __func__, "eComponentNotFound");
			}
			pEvent->Show("*Begin:Scheduler::RunOnce(ComponentId)", this->GetComponentId());
			pTargetComponent->BeginSequence(pEvent);
			pTargetComponent->ProcessAEvent(pEvent);
			pTargetComponent->EndSequence(pEvent);
			pEvent->Show("*End:Scheduler::RunOnce", this->GetComponentId());
		}

	}
	catch (Exception& exception) {
		exception.Println();
	}
}

void Scheduler::AllocateAComponent(Component* pComponent) {
	pComponent->SetPEventQueue(this->GetPEventQueue());
	m_mComponents.Add(pComponent->GetComponentId(), pComponent);
	LOG_NEWLINE("Scheduler::AllocateAComponent"
		, Directory::s_dirComponents[this->GetComponentId()]
		, Directory::s_dirComponents[pComponent->GetComponentId()]
	);
}
void Scheduler::DellocateAComponent(Component* pComponent) {
	m_mComponents.Remove(pComponent->GetComponentId());
}


/// Event Mapping Functions: Unmarshalling
void Scheduler::InitializeAsAScheduler(Event* pEvent) {
	unsigned uPriority = (unsigned)pEvent->GetlArg();
	this->InitializeAsAScheduler(uPriority);
}
void Scheduler::FinalizeAsAScheduler(Event* pEvent) {
	this->FinalizeAsAScheduler();
}

void Scheduler::AllocateAComponent(Event* pEvent) {
	ParamAllocateAComponent* pParamAllocateAComponent = (ParamAllocateAComponent*)pEvent->GetPArg();
	Component* pComponentAllocated = pParamAllocateAComponent->GetPComponentAllocated();
	if (pComponentAllocated == nullptr) {
		throw Exception((int)IScheduler::EError::eNullPoint, this->GetClassName(), __func__, "eNullPoint");
	}
	else {
		this->AllocateAComponent(pComponentAllocated);
	}
}
void Scheduler::DellocateAComponent(Event* pEvent) {
	ParamAllocateAComponent* pParamAllocateAComponent = (ParamAllocateAComponent*)pEvent->GetPArg();
	DellocateAComponent(pParamAllocateAComponent->GetPComponentAllocated());
}


void Scheduler::ProcessAEvent(Event* pEvent)
{
	switch (pEvent->GetType()) {
	case (unsigned)IScheduler::EEventType::eInitializeAsAScheduler:
		this->InitializeAsAScheduler(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eFinalizeAsAScheduler:
		this->FinalizeAsAScheduler(pEvent);
		break;

	case (unsigned)IScheduler::EEventType::eAllocateAComponent:
		this->AllocateAComponent(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eDellocateAComponent:
		this->DellocateAComponent(pEvent);
		break;

	case (unsigned)IScheduler::EEventType::eFork:
		this->Fork(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eJoin:
		this->Join(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eStart:
		this->StartAsAScheduler(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eStop:
		this->StopAsAScheduler(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::ePause:
		this->PauseAsAScheduler(pEvent);
		break;
	case (unsigned)IScheduler::EEventType::eResume:
		this->ResumeAsAScheduler(pEvent);
		break;
		
	default:
		Component::ProcessAEvent(pEvent);
		break;
	}
}