#pragma once

#include <12PPlatform/typedef.h>
#define _PEventQueue_Id _GET_CLASS_UID(_EPPlatform::_ePEventQueue)
#define _PEventQueue_Name "PEventQueue"


#include <02Platform/EventQueue/EventQueue.h>
#include <pthread.h>

#define MAXLENGTH_EVENTQUEUE 20

class PEventQueue: public EventQueue {
private:
	pthread_mutex_t m_mutex;

protected:
	void Lock() {
//		EnterCriticalSection(&CriticalSection);
        pthread_mutex_lock(&m_mutex);
	}
	void UnLock() {
//		LeaveCriticalSection(&CriticalSection);
        pthread_mutex_unlock(&m_mutex);
	}
public:
	PEventQueue(
		int nSchedulerId,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name)
		: EventQueue(nSchedulerId, nClassId, pcClassName)
	{
		int result = pthread_mutex_init(&m_mutex, nullptr);
		LOG_NEWLINE("PEventQueue::pthread_mutex_init(result)", result);
	}
	virtual ~PEventQueue() 
	{
		pthread_mutex_destroy(&m_mutex);
	}

	void Initialize() override {
		EventQueue::Initialize();
	}
	void Finalize() override {
		EventQueue::Finalize();        
	}

	void PushBack(Event* pEvent) override {
		Lock();
		EventQueue::PushBack(pEvent);
		UnLock();
	}
	Event* PopFront() override {
		Lock();
		Event *pEvent = EventQueue::PopFront();
		UnLock();
		return pEvent;
	}
};
