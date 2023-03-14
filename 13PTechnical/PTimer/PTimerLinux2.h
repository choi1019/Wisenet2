#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux2_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux2)
#define _PTimerLinux2_Name "PTimerLinux2"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>

#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

class PTimerLinux2 : public Timer, public PThread {
public:
	static int s_counterId;
private:
	int m_nId;

	long long m_msecInterval;
	unsigned m_secInterval;
	unsigned m_uCounter;

    timer_t m_timer;
    struct sigaction m_sigAction;
    struct sigevent m_sigEvent;
    struct itimerspec m_intervalTimerSpec;
	sigset_t m_mask;

	pthread_mutex_t m_mutex;

public:
	PTimerLinux2(size_t szPeriod, int nComponentId = _PTimerLinux2_Id, const char* sComponentName = _PTimerLinux2_Name);

	~PTimerLinux2() override;

	void Start();
	void Stop();

	void RunThread();
	void Signal();
	void TimeOut(Event *pEvent) override;

	void ProcessAEvent(Event *pEvent) override;
};