#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerLinux_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerLinux)
#define _PTimerLinux_Name "PTimerLinux"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>

class PTimerLinux : public Timer, public PThread {
public:
	static PTimerLinux *s_pPTimerLinux;
	static int s_counterId;

private:
	int m_nId;

	unsigned m_secInterval;
	long long m_msecInterval;
	unsigned m_uCounter;

	pthread_mutex_t m_mutex;

public:
	PTimerLinux(size_t msecInterval, int nComponentId = _PTimerLinux_Id, const char* sComponentName = _PTimerLinux_Name);

	~PTimerLinux() override;
	void Initialize() override;
	void Finalize() override;

	void Start();
	void Stop();

	void RunThread() override;
	void Signal();
	void TimeOut(Event *pEvent) override;

	void ProcessAEvent(Event *pEvent) override;
};