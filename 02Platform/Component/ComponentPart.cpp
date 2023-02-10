#include <02Platform/Component/ComponentPart.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <01Base/Aspect/Directory.h>
#include <01Base/StdLib/String.h>
#include <02Platform/EventQueue/EventQueue.h>

ComponentPart::ComponentPart(unsigned uClassId, const char* acClassName)
	: BaseObject(uClassId, acClassName)
	, m_pUId(nullptr)
	, m_pmReceivers(nullptr)
	, m_pmTargetsGroups(nullptr)
//	, m_pSequenceHead(nullptr)
	, m_pEventParent(nullptr) 
{
	this->RegisterExceptions();
}

ComponentPart::~ComponentPart() {
}

void ComponentPart::Initialize() {
	BaseObject::Initialize();
}

void ComponentPart::Finalize() {
	BaseObject::Finalize();
}

void ComponentPart::RegisterExceptions() {
	Directory::s_dirExceptions[(unsigned)EException::eNotAllocated] = "eNotAllocated";
	Directory::s_dirExceptions[(unsigned)EException::eReceiverNotFound] = "eReceiverNotFound";
	Directory::s_dirExceptions[(unsigned)EException::eNotAssociated] = "eNotAssociated";
	Directory::s_dirExceptions[(unsigned)EException::eNotTargeted] = "eNotTargeted";
	Directory::s_dirExceptions[(unsigned)EException::eEventNotSupported] = "eEventNotSupported";
}

UId ComponentPart::FindUid(int nReceiverName) {
	auto iterator = this->m_pmReceivers->Find(nReceiverName);
	if (iterator == m_pmReceivers->end()) {
		throw Exception(
			(unsigned)EException::eNotAssociated,
			this->GetClassName(),
			__func__,
			String("Receiver(") + String(nReceiverName) + ") is not Associated"
		);
	}
	return iterator->second;
}

// for transaction
void ComponentPart::BeginSequence(Event* pEvent) {
	m_pEventParent = nullptr;
	if (pEvent->IsReply()) {
		// ?
	} else {
		if (pEvent->IsSynchronous()) {
			// for nested events
			m_pEventParent = pEvent;
		}
	}
}

void ComponentPart::EndSequence(Event*pEvent) {
	if (pEvent->IsReply()) {
		// nested
		if (pEvent->IsNested()) {
			pEvent->GetPParent()->DecrementCountChildren();
			if (pEvent->GetCoundChildren() == 0) {
				ReplyEvent(pEvent->GetPParent());
			}
		} else {
			delete pEvent;	
		}
	} else {
		// no child, synchronous
		if (pEvent->IsSynchronous()) {
			if (pEvent->GetCoundChildren() == 0) {
				ReplyEvent(pEvent->GetPParent());
			}
		// asyncrhonous
		} else {
			delete pEvent;	
		}
	}
}

///////////////////////////////////////
// send a event
///////////////////////////////////////
void ComponentPart::SendAEvent(Event* pEvent) {
	if (pEvent->GetUIdTarget().GetPEventQueue() == nullptr) {
		throw Exception(
			(unsigned)EException::eNotAllocated,
			this->GetClassName(),
			__func__,
			"EventQueue is not allocated"
		);
	}
	// parent event should wait for all children events finished
	if (m_pEventParent->IsSynchronous()) {
		// for nesting
		m_pEventParent->IncrementCountChildren();
		pEvent->SetPParent(m_pEventParent);
		pEvent->SetBNested(true);
	}
	// push event to a target event queue
	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
}

///////////////////////////////////////
// reply a event
///////////////////////////////////////
void ComponentPart::ReplyEvent(Event* pEvent, long long lArg, ValueObject* pArg) {
	// set pEvent as a Reply
	pEvent->SetBReply(true);
	pEvent->SetReplyType(pEvent->GetType());
	// swap source and destination
	pEvent->SetUIdTarget(pEvent->GetUIdSource());
	pEvent->SetUIdSource(*m_pUId);
	// reply argument
	pEvent->SetlArg(lArg);
	pEvent->SetPArg(pArg);
	// Send event
	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
}

///////////////////////////////////////
// send a synchronous event
///////////////////////////////////////
void ComponentPart::SendReplyEvent(UId uIdTarget, int nEventType, long long lArg, ValueObject* pArg, int ReplyType)
{
	Event* pEvent = new("") Event(*m_pUId, uIdTarget, nEventType, lArg, pArg, ReplyType);
	pEvent->SetBSynchronous(true);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendReplyEvent(int nReceiverName, int nEventType, long long lArg, ValueObject* pArg, int ReplyType)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendReplyEvent(uIdTarget, nEventType, lArg, pArg, ReplyType);
}

///////////////////////////////////////
// send an asynchronous event
///////////////////////////////////////
void ComponentPart::SendNoReplyEvent(UId uIdTarget, int nEventType, long long lArg, ValueObject* pArg)
{
	Event* pEvent = new("") Event(*m_pUId, uIdTarget, nEventType, lArg, pArg);
	pEvent->SetBSynchronous(false);
	this->SendAEvent(pEvent);
}
void ComponentPart::SendNoReplyEvent(int nReceiverName, int nEventType, long long lArg, ValueObject* pArg)
{
	UId uIdTarget = this->FindUid(nReceiverName);
	this->SendNoReplyEvent(uIdTarget, nEventType, lArg, pArg);
}

///////////////////////////////////////
// semd target events
///////////////////////////////////////
void ComponentPart::SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg, ValueObject* pArg) {
	for (auto& itr : *(this->m_pmTargetsGroups->Find(groupName)->second))
	{
		this->SendNoReplyEvent(itr, eventType, lArg, pArg);
	}
}

