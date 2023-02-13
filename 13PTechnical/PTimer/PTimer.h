#pragma once

#include <13PTechnical/typedef.h>
#define _PTimer_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePTimer)
#define _PTimer_Name "PTimer"

#include <03Technical/Timer/Timer.h>
#include <13PTechnical/PThread/PThread.h>
#include <linux/rtc.h>

class PTimer : public Timer, public PThread {
private:
	struct rtc_time m_time;
	int m_nFd;
	size_t m_szPeriod;
	unsigned m_uCounter;
	
public:
	PTimer(size_t szPeriod, int nComponentId = _PTimer_Id, const char* sComponentName = _PTimer_Name);

	~PTimer() override;
	void Initialize() override;
	void Finalize() override;

	void Start() override;
	void Stop() override;

	void TimeOut(Event *pEvent) override;
	void RunThread() override;

	void ProcessAEvent(Event *pEvent) override;
};