#include <02Platform/Component/Component.h>

#include <01Base/Aspect/Directory.h>
#include <02Platform/EventQueue/EventQueue.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <03Technical/MemoryManager/SlotInfo.h>

//unsigned Component::s_uCounter = 0;

Component::Component(unsigned uClassId, const char* sClassName)
	: ComponentPart(uClassId, sClassName)
{
	this->SetPUId(new("UId") UId(this->GetObjectId(), nullptr));
	// component id directory
	Directory::s_dirComponents[this->GetComponentId()] = sClassName;
	this->RegisterEventTypes();
	this->RegisterExceptions();

	this->SetPMReceivers(new("ReceiversMap") MapReceivers());
	this->SetPMTargetsGroups(new("TargetsMap") MapTargetsGroups());
	// default target
	this->AssociateAReceiver((int)EReceivers::eThis, this->GetUId());
	// state
	SetEState(IComponent::EState::eCreated);
}

Component::~Component() {
	for (MapPair<int, ComponentPart*> itrComponentPart: m_mapComponentParts) {
		delete itrComponentPart.second;
	}
	delete this->m_pUId;
	delete this->m_pmReceivers;
	// delete target group vectors
	for (auto itr : *this->m_pmTargetsGroups) {
		delete itr.second;
	}
	delete this->m_pmTargetsGroups;
}

void Component::RegisterEventTypes() {
	Directory::s_dirEvents[(unsigned)EEventType::eAssociateAReceiver] = "eAssociateAReceiver";
	Directory::s_dirEvents[(unsigned)EEventType::eAssociateATarget] = "eAssociateATarget";
	// Directory::s_dirEvents[(unsigned)EEventType::eInitialize] = "eInitialize";
	// Directory::s_dirEvents[(unsigned)EEventType::eFinalize] = "eFinalize";
	// Directory::s_dirEvents[(unsigned)EEventType::eStart] = "eStart";
	// Directory::s_dirEvents[(unsigned)EEventType::eStop] = "eStop";
	// Directory::s_dirEvents[(unsigned)EEventType::eRun] = "eRun";
	// Directory::s_dirEvents[(unsigned)EEventType::ePause] = "ePause";
}

void Component::RegisterExceptions() {
	Directory::s_dirExceptions[(unsigned)EException::eNotAllocated] = "eNotAllocated";
	Directory::s_dirExceptions[(unsigned)EException::eReceiverNotFound] = "eReceiverNotFound";
	Directory::s_dirExceptions[(unsigned)EException::eNotAssociated] = "eNotAssociated";
	Directory::s_dirExceptions[(unsigned)EException::eNotTargeted] = "eNotTargeted";
	Directory::s_dirExceptions[(unsigned)EException::eEventNotSupported] = "eEventNotSupported";
}

void Component::AddPart(unsigned uName, ComponentPart* pComponentPart) {
	pComponentPart->SetPUId(this->GetPUId());
	pComponentPart->SetPMReceivers(this->GetPMReceivers());
	pComponentPart->SetPMTargetsGroups(this->GetPMTargetsGroups());

	this->m_mapComponentParts.Add(uName, pComponentPart);
}
ComponentPart* Component::GetPart(unsigned uName) {
	return this->m_mapComponentParts[uName];
}
ComponentPart* Component::RemovePart(unsigned uName) {
	ComponentPart* pComponentPart = this->m_mapComponentParts[uName];
	if (pComponentPart->GetEState() == IComponent::EState::eRunning) {
		throw Exception(
			(unsigned)EException::eNotStopped,
			this->GetClassName(),
			__func__,
			String("ComponentPart(") + String(uName) + ") is not terminated"
		);
	}
	this->m_mapComponentParts.Remove(uName);
	return pComponentPart;
}
void Component::RemovePartAll() {
	for (auto iter : this->m_mapComponentParts) {
		ComponentPart* pComponentPart = iter.second;
		if (pComponentPart->GetEState() == IComponent::EState::eRunning) {
			throw Exception(
				(unsigned)EException::eNotStopped,
				this->GetClassName(),
				__func__,
				String("ComponentPart(") + String(iter.first) + ") is not terminated"
			);
		}
	}
	this->m_mapComponentParts.Clear();
}

void Component::SetPEventQueue(EventQueue* pEventQueue) {
	this->GetPUId()->SetPEventQueue(pEventQueue);
	SetEState(IComponent::EState::eAllocated);
	LOG_NEWLINE("Component::SetPEventQueue", this->GetClassName(), pEventQueue->GetClassId());
}

void Component::AssociateAReceiver(unsigned receiverName, UId receiverUId) {
	this->GetPMReceivers()->Add(receiverName, receiverUId);
	LOG_NEWLINE("Component::AssociateAReceiver"
		, Directory::s_dirComponents[this->GetComponentId()]
		, Directory::s_dirComponents[receiverUId.GetComponentId()]);
}

void Component::AssociateATarget(unsigned uGroupName, Vector<UId>& vNewUIdTargetComponents) {
	// find a groupName
	auto itrTargetGroup = this->GetPMTargetsGroups()->Find(uGroupName);
	if (itrTargetGroup == this->GetPMTargetsGroups()->end()) {
		// if not exist, create a new one
		itrTargetGroup->first = uGroupName;
		itrTargetGroup->second = new("Vector") Vector<UId, MAXTARGETCOMPONENTS>();
		this->GetPMTargetsGroups()->Add(*itrTargetGroup);
	}
	for (auto itrNewUIdTargetComponent : vNewUIdTargetComponents) {
		bool bExist = false;
		for (UId uIdTargetComponent : *(itrTargetGroup->second)) {
			if (uIdTargetComponent == itrNewUIdTargetComponent) {
				// arleady exist
				bExist = true;
				break;
			}
		}
		if (!bExist) {
			// if newTargetUId is new one, then add
			itrTargetGroup->second->Add(itrNewUIdTargetComponent);
			LOG(Directory::s_dirComponents[this->GetComponentId()], __func__
				, uGroupName
				, Directory::s_dirComponents[itrNewUIdTargetComponent.GetComponentId()]);
		}
	}
}

// void Component::Initialize() {
// 	ComponentPart::Initialize();
// 	SetEState(IComponent::EState::eInitialized);
// 	LOG_NEWLINE(Directory::s_dirComponents[this->GetComponentId()], __func__);
// }
// void Component::Finalize() {
// 	ComponentPart::Finalize();
// 	SetEState(IComponent::EState::eFinalized);
// 	LOG_NEWLINE(Directory::s_dirComponents[this->GetComponentId()], __func__);
// }
// void Component::Start() {
// 	SetEState(IComponent::EState::eRunning);
// }
// void Component::Stop() {
// 	SetEState(IComponent::EState::eStopped);
// 	this->RemovePartAll();
// }
// void Component::Run() {
// }
// void Component::Pause() {
// 	SetEState(IComponent::EState::ePaused);
// }

/////////////////////////////////////////////////////////////
// event mapping functions
/////////////////////////////////////////////////////////////
void Component::AssociateAReceiver(Event* pEvent) {
	if (GetEState() < IComponent::EState::eAllocated) {
		throw Exception((unsigned)EException::eNotAllocated, this->GetClassName(), __func__, "eNotAllocated");
	}

	ParamAssociateAReceiver* pParamAssociateAReceiver = (ParamAssociateAReceiver*)pEvent->GetPArg();
	unsigned uReceiverName = pParamAssociateAReceiver->GetReceiverName();
	UId uIdReceiverComponent = pParamAssociateAReceiver->GetUIdReceiverComponent();
	this->AssociateAReceiver(uReceiverName, uIdReceiverComponent);

	SetEState(IComponent::EState::eAssociated);
	//		Log(this->GetClassName(), "AssociateAReceiver", uReceiverName).Println();
}

void Component::AssociateATarget(Event* pEvent) {
	if (GetEState() < IComponent::EState::eAllocated) {
		throw Exception((unsigned)EException::eNotAllocated, this->GetClassName(), __func__, "eNotAllocated");
	}

	ParamAssociateATarget* pParamAssociateATarget = (ParamAssociateATarget*)pEvent->GetPArg();
	unsigned uGroupName = pParamAssociateATarget->GetGroupName();
	Vector<UId> vUIdTargetComponent = pParamAssociateATarget->GetVUIdTargetComponents();
	this->AssociateATarget(uGroupName, vUIdTargetComponent);

	//		this->SendAReplyEvent(pEvent, 0, nullptr);
	SetEState(IComponent::EState::eTargeted);
}

// void Component::Initialize(Event* pEvent) {
// 	this->Initialize();
// }
// void Component::Finalize(Event* pEvent) {
// 	this->Finalize();
// }

// void Component::Start(Event* pEvent) {
// 	this->Start();
// }

// void Component::Stop(Event* pEvent) {
// 	this->Stop();
// }

// void Component::Run(Event* pEvent) {
// 	this->Run();
// }
// void Component::Pause(Event* pEvent) {
// 	this->Pause();
// }

void Component::BeginSequence(Event* pEvent) {
	ComponentPart::BeginSequence(pEvent);
}

void Component::EndSequence(Event* pEvent) {
	ComponentPart::EndSequence(pEvent);
}

void Component::ProcessAEvent(Event* pEvent) {
	switch (pEvent->GetType()) {
	case (unsigned)EEventType::eAssociateAReceiver:
		this->AssociateAReceiver(pEvent);
		break;
	case (unsigned)EEventType::eAssociateATarget:
		this->AssociateATarget(pEvent);
		break;
	// case (unsigned)EEventType::eInitialize:
	// 	this->Initialize(pEvent);
	// 	break;
	// case (unsigned)EEventType::eStart:
	// 	this->Start(pEvent);
	// 	break;
	// case (unsigned)EEventType::eStop:
	// 	this->Stop(pEvent);
	// 	break;
	// case (unsigned)EEventType::eRun:
	// 	this->Run(pEvent);
	// 	break;
	// case (unsigned)EEventType::ePause:
	// 	this->Pause(pEvent);
	// 	break;
	// case (unsigned)EEventType::eFinalize:
	// 	this->Finalize(pEvent);
	// 	break;
	default:
		throw EXCEPTION((int)EException::eEventNotSupported);
		break;
	}
}
