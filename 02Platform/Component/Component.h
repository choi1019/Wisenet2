#pragma once

#include <02Platform/Component/ComponentPart.h>
class Component : public ComponentPart
{
public:
	typedef MapPair<int, ComponentPart*> PairComponentPart;
	typedef Map<int, ComponentPart*> MapComponentParts;
private:
	MapComponentParts m_mapComponentParts;

public:
	Component(unsigned uClassId = Component_Id, const char* pcClassName = Component_Name);
	virtual ~Component();
	
	void AddPart(unsigned uName, ComponentPart* pComponentPart);
	ComponentPart* GetPart(unsigned uName);
	ComponentPart* RemovePart(unsigned uName);
	void RemovePartAll();
	MapComponentParts *GetPComponentParts() { return &m_mapComponentParts; }
	
	void SetPEventQueue(EventQueue* pEventQueue);

	void BeginSequence(Event* pEvent);
	virtual void ProcessAEvent(Event* pEvent);
	void EndSequence(Event* pEvent);
	
public:
	virtual void RegisterEventTypes();
	virtual void RegisterExceptions();

	virtual void AssociateAReceiver(unsigned receiverName, UId receiverUId);
	virtual void AssociateATarget(unsigned uGroupName, Vector<UId>& vNewUIdTargetComponents);

	// virtual void Initialize();
	// virtual void Finalize();

	// virtual void Start();
	// virtual void Stop();
	// virtual void Pause();
	// virtual void Run();

protected:
	virtual void AssociateAReceiver(Event* pEvent);
	virtual void AssociateATarget(Event* pEvent);

	// virtual void Initialize(Event* pEvent);
	// virtual void Finalize(Event* pEvent);

	// virtual void Start(Event* pEvent);
	// virtual void Stop(Event* pEvent);
	// virtual void Run(Event* pEvent);
	// virtual void Pause(Event* pEvent);
};
