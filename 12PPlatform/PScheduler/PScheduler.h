#pragma once

#include <12PPlatform/typedef.h>
#define _PScheduler_Id _GET_CLASS_UID(_EPPlatform::_ePScheduler)
#define _PScheduler_Name "PScheduler"

#include <02Platform/Scheduler/Scheduler.h>
#include <12PPlatform/PEventQueue/PEventQueue.h>

#define PRIORITY 100

class PScheduler : public Scheduler
{
public:
	PScheduler(
		int uClassId = _PScheduler_Id,
		const char* pcClassName = _PScheduler_Name);
	virtual ~PScheduler();

	virtual void InitializeAsAScheduler(int uPriority);
	virtual void FinalizeAsAScheduler();

	virtual void Start();
	virtual void Join();

protected:
	virtual void Initialize();
	virtual void Finalize();

//	void RunOnce();
};