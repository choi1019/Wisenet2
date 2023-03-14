#pragma once

#include <13PTechnical/typedef.h>
#define _PTimerRTC_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimerRTC)
#define _PTimerRTC_Name "PTimerRTC"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>
#include <linux/rtc.h>

extern void* CallBackPTimerRTC(void *pObject);

class PTimerRTC : public Timer, public PThread {
private:
	struct rtc_time m_time;
	int m_nFd;
	size_t m_szPeriod;
	unsigned m_uCounter;
	
public:
	PTimerRTC(size_t szPeriod, int nComponentId = _PTimerRTC_Id, const char* sComponentName = _PTimerRTC_Name);

	~PTimerRTC() override;
	void Initialize() override;
	void Finalize() override;

	void Start();
	void Stop();

	void TimeOut(Event *pEvent) override;
	void RunThread();

	void ProcessAEvent(Event *pEvent) override;
};