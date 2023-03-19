#pragma once

#include <03Technical/Timer/ITimer.h>
#include <02Platform/Component/Component.h>
#include <01Base/Object/ValueObject.h>

class Timer : public Component, public ITimer {
public:
	class ParamStartTimer : public ValueObject {
	private:
		int m_nDueTime;
		int m_nInterval;
	public:
		ParamStartTimer(int m_nDueTime, int nInterval)
			: m_nDueTime(undefined)
			, m_nInterval(undefined)
		{
			this->m_nInterval = nInterval;
		}
		~ParamStartTimer() {}

		int GetDueTime() {
			return m_nDueTime;
		}
		int GetInterval() {
			return m_nInterval;
		}
	};

public:
	Timer(int nComponentId = _Timer_Id, const char* sComponentName = _Timer_Name) 
	: Component(nComponentId, sComponentName) {
	}
	~Timer() {}

	virtual void TimeOut(Event *pEvent) = 0;
};

