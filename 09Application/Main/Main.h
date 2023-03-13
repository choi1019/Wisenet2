#pragma once

#include <09Application/Main/IMain.h>
#include <02Platform/Scheduler/Scheduler.h>

#include <09Application/LifecycleManager/LifecycleManager.h>

class Main : public Scheduler, public IMain
{
private:
	LifecycleManager* m_pLifecycleManager;

	void RegisterEventTypes();
	void RegisterExceptions();

public:
	Main(
		EventQueue *pEventQueue,
		unsigned uClassId = _Main_Id,
		const char* pcClassName = _Main_Name);
	virtual ~Main();

	// as a Main
	virtual void BootstrapSystem() = 0;
	void InitializeAsAMain(Event* pEvent);
	virtual void RunAsAMain();
	void FinalizeAsAMain(Event* pEvent);
	virtual void ShutdownSystem() = 0;

	// as a Scheduler - do nothing except "RunAsAScheduler"
	void InitializeAsAScheduler(int uPriority) override {
		Scheduler::InitializeAsAScheduler(uPriority);
	}
	void FinalizeAsAScheduler() override {
		Scheduler::FinalizeAsAScheduler();
	}
	// main is not a normal scheduler
	void Fork() override {}
	void Join() override {}
	void StartAsAScheduler() override {}
	void PauseAsAScheduler() override {}
	void ResumeAsAScheduler() override {}
	void StopAsAScheduler() override {}

	// as a Component
	virtual void Initialize(Event *pEvent) { Scheduler::Initialize(pEvent); }
	virtual void Finalize(Event *pEvent) { Scheduler::Initialize(pEvent); }

protected:
	LifecycleManager* GetPLifecycleManager() { return this->m_pLifecycleManager; }
	void SetPLifecycleManager(LifecycleManager* pLifecycleManager) { this->m_pLifecycleManager = pLifecycleManager; }
	
	virtual void ProcessAEvent(Event* pEvent);
};

