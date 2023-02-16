#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux2_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux2)
#define _PTimerLinux2_Name "PTimerLinux2"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>

#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

class PTimerLinux2 : public Timer, public PThread {
public:
	static int s_counterId;
private:
	int m_nId;
	timer_t *m_pIdTimer;

	long long m_msecInterval;
	unsigned m_secInterval;
	unsigned m_uCounter;

	struct sigevent m_sigEvent;
	struct sigaction m_sigAction;
	struct itimerspec m_iTimerSpec;

	pthread_mutex_t m_mutex;

public:
	PTimerLinux2(size_t szPeriod, int nComponentId = _PTimerLinux2_Id, const char* sComponentName = _PTimerLinux2_Name);

	~PTimerLinux2() override;
	void Initialize() override;
	void Finalize() override;

	void Start() override;
	void Stop() override;

	void RunThread();
	void Signal();
	void TimeOut(Event *pEvent) override;

	void ProcessAEvent(Event *pEvent) override;
};