#pragma once

#include <02Platform/typedef.h>
#define ComponentPart_Id _GET_CLASS_UID(_ELayer_Platform::_eComponentPart)
#define ComponentPart_Name "ComponentPart"  // class string name

#include <02Platform/Component/IComponent.h>
#include <01Base/Object/BaseObject.h>
#include <02Platform/Component/UId.h>
#include <02Platform/EventQueue/Event.h>

#include <01Base/StdLib/Map.h>
#include <01Base/StdLib/Vector.h>

#undef GetClassName

class ComponentPart : public BaseObject, public IComponent {
protected:
	//private:
		// UId, Component Id, EventQueue *
	UId* m_pUId;
	IComponent::EState m_eState;

	// <receiverName, receiverComponent UId>
	Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* m_pmReceivers;
	// <targetGroupName, vector<targetUId>>
	Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* m_pmTargetsGroups;

	// for nested event
	Event *m_pEventParent;

public:
	// for Component
	ComponentPart(
		unsigned uClassId = ComponentPart_Id,
		const char* acClassName = ComponentPart_Name);
	virtual ~ComponentPart();
	virtual void Initialize();
	virtual void Finalize();

	// getters and setters
	virtual IComponent::EState GetEState() { return this->m_eState; }
	virtual void SetEState(IComponent::EState eState) { this->m_eState = eState; }

	UId* GetPUId() { return m_pUId; }
	UId GetUId() { return *m_pUId; }
	void SetPUId(UId* pUId) { m_pUId = pUId; }
	int GetComponentId() { return m_pUId->GetComponentId(); }
	void SetComponentId(int uComponentId) { return m_pUId->SetComponentId(uComponentId); }

	Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* GetPMReceivers() { return m_pmReceivers; }
	void SetPMReceivers(Map<unsigned, UId, MAXRECEIVERCOMPONENTS>* pmReceivers) { this->m_pmReceivers = pmReceivers; }
	Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* GetPMTargetsGroups() { return this->m_pmTargetsGroups; }
	void SetPMTargetsGroups(Map<unsigned, Vector<UId, MAXTARGETCOMPONENTS>*, MAXTARGETGROUPS>* pmTargetsGroups) { m_pmTargetsGroups = pmTargetsGroups; }

	// methods
	UId FindUid(int nReceiverName);

	// for sequencing events
	void BeginSequence(Event* pEvent);
	void EndSequence(Event*pEvent);

protected:
	void SendAEvent(Event* pEvent);
	void ReplyEvent(Event* pEvent);

	void SendReplyEvent(UId uIdTarget, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr, ValueObject *pIterator = nullptr);
	void SendReplyEvent(int nReceiverName, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr);
	void SendReplyEventIteration(UId uIdTarget, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr, ValueObject *pIterator = nullptr);
	void SendReplyEventIteration(int nReceiverName, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr, ValueObject *pIterator = nullptr);

	void SendNoReplyEvent(UId uIdTarget, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr);
	void SendNoReplyEvent(int nReceiverName, int nEventType, long long lArg = 0, ValueObject* pArg = nullptr);
	void SendTargetEvents(unsigned groupName, unsigned eventType, long long lArg = 0, ValueObject* pArg = nullptr);
};