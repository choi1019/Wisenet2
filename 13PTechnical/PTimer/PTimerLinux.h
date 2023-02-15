#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux)
#define _PTimerLinux_Name "PTimerLinux"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>
//#include <linux/hrtimer.h>

#define TIMER_NUMMAX 4
extern void* CallBackPTimerLinux(void *pObject);
//static void SignalPTimerLinux0(int signum);
extern void(*CallbackSignal[TIMER_NUMMAX])(int);

class PTimerLinux : public Timer, public PThread {
public:
	static PTimerLinux *s_apPTimer[TIMER_NUMMAX];
	static int s_counterId;
private:
	int m_nId;
	unsigned m_msecInterval;
	unsigned m_secInterval;
	unsigned m_uCounter;

	pthread_mutex_t m_mutex;		
public:
	PTimerLinux(size_t szPeriod, int nComponentId = _PTimerLinux_Id, const char* sComponentName = _PTimerLinux_Name);

	~PTimerLinux() override;
	void Initialize() override;
	void Finalize() override;

	void Start() override;
	void Stop() override;

	void RunThread() override;
	void Signal();
	void TimeOut(Event *pEvent) override;

	void ProcessAEvent(Event *pEvent) override;
};