#pragma once

#include <12PPlatform/typedef.h>
#define _PEventQueue_Id _GET_CLASS_UID(_EPPlatform::_ePEventQueue)
#define _PEventQueue_Name "PEventQueue"


#include <02Platform/EventQueue/EventQueue.h>

#define MAXLENGTH_EVENTQUEUE 20

class PEventQueue: public EventQueue {
private:

public:
	PEventQueue(
		int nSchedulerId,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name)
		: EventQueue(nSchedulerId, nClassId, pcClassName)
	{
		// Create a semaphore with initial and max counts of MAX_SEM_COUNT

	}

	virtual ~PEventQueue() 
	{
	}

	void PushBack(Event* pEvent) override {
		PEventQueue::PushBack(pEvent);
	}

	Event* PopFront() override {
		Event *pEvent = PEventQueue::PopFront();
		return pEvent;
	}
};
