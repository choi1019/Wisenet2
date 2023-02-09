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
		// nested
		if (pEvent->GetPParent() != nullptr) {
			pEvent->GetPParent()->DecrementCountChildren();
		}
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
		if (pEvent->GetPParent() != nullptr) {
			pEvent->GetPParent()->DecrementCountChildren();
			if (pEvent->GetCoundChildren() == 0) {
				ReplyEvent(pEvent->GetPParent());
			}
		} else {
			delete pEvent;	
		}
	} else {
		if (pEvent->IsSynchronous()) {
			if (pEvent->GetCoundChildren() == 0) {
				ReplyEvent(pEvent->GetPParent());
			}
		} else {
			delete pEvent;	
		}
	}
}

///////////////////////////////////////
// prepare event
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

	if (m_pEventParent->IsSynchronous()) {
		// for nesting
		pEvent->SetBNested(true);
		pEvent->SetPParent(m_pEventParent);
		pEvent->GetPParent()->IncrementCountChildren();

	}
	
	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
	
	// if (pEvent->IsSequential()) {
	// 	// for sequencing
	// 	// add current event at the end
	// 	if (m_pEventSequenceFront == nullptr) {
	// 		m_pEventSequenceFront = pEvent;
	// 		m_pEventSequenceRear = pEvent;
	// 	} else {
	// 		pEvent->SetPSequenceNext(m_pEventSequenceRear->GetPSequenceNext());
	// 		m_pEventSequenceRear = pEvent;
	// 	}
	// } else {
	// 	// Send event
	// 	pEvent->GetUIdTarget().GetPEventQueue()->PushBack(pEvent);
	// }
}

///////////////////////////////////////
// reply event
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
// Synchronous event
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
// Asynchronous event
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
// target events
///////////////////////////////////////
void ComponentPart::SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg, ValueObject* pArg) {
	for (auto& itr : *(this->m_pmTargetsGroups->Find(groupName)->second))
	{
		this->SendNoReplyEvent(itr, eventType, lArg, pArg);
	}
}

