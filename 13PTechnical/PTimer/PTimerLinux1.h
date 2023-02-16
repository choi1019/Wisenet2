#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux1_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux1)
#define _PTimerLinux1_Name "PTimerLinux1"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>

#include <unistd.h>
#include <sys/time.h>
#include <signal.h>

static void*(CallBackPTimerLinux1)(void *pObject);
static void (SignalPTimerLinux1)(int);

class PTimerLinux1 : public Timer, public PThread {
public:
	static PTimerLinux1 *s_pPTimerLinux1;
	static int s_counterId;
private:
	int m_nId;
	timer_t m_idTimer;

	long long m_msecInterval;
	unsigned m_secInterval;
	unsigned m_uCounter;

	struct sigevent m_sigEvent;
	struct sigaction m_sigAction;
	struct itimerspec m_iTimerSpec;

	pthread_mutex_t m_mutex;

public:
	PTimerLinux1(size_t szPeriod, int nComponentId = _PTimerLinux1_Id, const char* sComponentName = _PTimerLinux1_Name);

	~PTimerLinux1() override;
	void Initialize() override;
	void Finalize() override;

	void Start() override;
	void Stop() override;

	void RunThread() override;
	void Signal();
	void TimeOut(Event *pEvent) override;

	void ProcessAEvent(Event *pEvent) override;
};