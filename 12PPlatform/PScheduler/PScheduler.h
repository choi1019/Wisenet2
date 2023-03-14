#pragma once

#include <12PPlatform/typedef.h>
#define _PScheduler_Id _GET_CLASS_UID(_EPPlatform::_ePScheduler)
#define _PScheduler_Name "PScheduler"

#include <02Platform/Scheduler/Scheduler.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>
#include <13PTechnical/PThread/PThread.h>

#define PRIORITY 100

class PScheduler : public Scheduler, public PThread
{
private:

public:
	PScheduler(
		int uClassId = _PScheduler_Id,
		const char* pcClassName = _PScheduler_Name);
	virtual ~PScheduler();

	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	void Fork() override;
	void Join() override;
	void RunThread() override;

protected:
	// virtual void Initialize(Event *pEvent);
	// virtual void Finalize(Event *pEvent);

//	void RunOnce();
};