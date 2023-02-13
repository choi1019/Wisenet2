#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux)
#define _PTimerLinux_Name "PTimerLinux"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>
#include <sys/time.h>

extern void* CallBackPTimerLinux(void *pObject);

class PTimerLinux : public Timer, public PThread {
public:
	static PTimerLinux *s_pPTimer;
	static pthread_mutex_t *s_pMutex;

private:
	size_t m_szPeriod;
	unsigned m_uCounter;


	
public:
	PTimerLinux(size_t szPeriod, int nComponentId = _PTimerLinux_Id, const char* sComponentName = _PTimerLinux_Name);

	~PTimerLinux() override;
	void Initialize() override;
	void Finalize() override;

	void Start() override;
	void Stop() override;
	void RunThread() override;

	void TimeOut(Event *pEvent) override;


	void ProcessAEvent(Event *pEvent) override;
};