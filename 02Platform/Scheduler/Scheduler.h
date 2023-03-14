#pragma once

#include <02Platform/EventQueue/EventQueue.h>
#include <02Platform/Scheduler/IScheduler.h>
#include <01Base/StdLib/Map.h>

#undef GetClassName

class Scheduler : public Component, public IScheduler
{
private:
	typedef Map<unsigned, Component*> ComponentMap;
	typedef Map<unsigned, Component*>::Iterator MapIterator;
	ComponentMap m_mComponents;

	EventQueue* m_pEventQueue;
	IScheduler::EState m_eState;

	virtual void RegisterEventTypes();

public:
	Scheduler(EventQueue *pEventQueue, unsigned classId = _Scheduler_Id, const char* className = _Scheduler_Name);
	virtual ~Scheduler();

protected:
	// virtual void Initialize(Event *pEvent);
	// virtual void Finalize(Event *pEvent);

public:
	EventQueue* GetPEventQueue();
	void SetPEventQueue(EventQueue* pEventQueue);

	// jobs to do before the thread is started
	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	virtual void Fork() {}
	virtual void Join() {}
	virtual void StartAsAScheduler();
	virtual void StopAsAScheduler();
	virtual void RunAsAScheduler();
	virtual void PauseAsAScheduler();
	virtual void ResumeAsAScheduler();


protected:
	virtual void RunOnce();

	virtual void Fork(Event* pEvent);
	virtual void Join(Event* pEvent);
	virtual void StartAsAScheduler(Event* pEvent);
	virtual void StopAsAScheduler(Event* pEvent);
	virtual void PauseAsAScheduler(Event* pEvent);
	virtual void ResumeAsAScheduler(Event* pEvent);

	void AllocateAComponent(Component* pComponent);
	void DellocateAComponent(Component* pComponent);

private:
	/// Event Mapping Functions: Unmarshalling
	void InitializeAsAScheduler(Event* pEvent);
	void FinalizeAsAScheduler(Event* pEvent);

	void AllocateAComponent(Event* pEvent);
	void DellocateAComponent(Event* pEvent);

public:
	virtual void ProcessAEvent(Event* pEvent);

};

