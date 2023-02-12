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

	virtual void InitializeVarialbes();
	virtual void DeleteVarialbes();

	void RegisterEventTypes();

public:
	Scheduler(unsigned classId = _Scheduler_Id, const char* className = _Scheduler_Name);
	virtual ~Scheduler();

protected:
	virtual void Initialize();
	virtual void Finalize();

public:
	EventQueue* GetPEventQueue();
	void SetPEventQueue(EventQueue* pEventQueue);

	// jobs to do before the thread is started
	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	virtual void Fork() {}
	virtual void Join() {}
	virtual void Start();
	virtual void Stop();
	virtual void Run();
	virtual void Pause();
	virtual void Resume();


protected:
	virtual void RunOnce();

	virtual void Fork(Event* pEvent);
	virtual void Join(Event* pEvent);
	virtual void Start(Event* pEvent);
	virtual void Stop(Event* pEvent);
	virtual void Pause(Event* pEvent);
	virtual void Resume(Event* pEvent);

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

