#pragma once

#include <12PPlatform/typedef.h>
#define _PEventQueue_Id _GET_CLASS_UID(_EPPlatform::_ePEventQueue)
#define _PEventQueue_Name "PEventQueue"


#include <02Platform/EventQueue/EventQueue.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <pthread.h>
#include <semaphore.h>

#define MAXLENGTH_EVENTQUEUE 20

class PEventQueue: public EventQueue {
private:
	pthread_mutex_t m_mutex;
	sem_t m_semaphoreFull;
	sem_t m_semaphoreEmpty;

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
		int nSchedulerId = UNDEFINED,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name)
		: EventQueue(nSchedulerId, nClassId, pcClassName)
	{
		int result = pthread_mutex_init(&m_mutex, nullptr);
		if (result != 0) {
			throw Exception(-1, "PEventQueue::PEventQueue");
		}
		result = sem_init(&m_semaphoreFull, 0, MAXLENGTH_EVENTQUEUE);
		if (result != 0) {
			throw Exception(-1, "PEventQueue::PEventQueue");
		}
		result = sem_init(&m_semaphoreEmpty, 0, 0);
		if (result != 0) {
			throw Exception(-1, "PEventQueue::PEventQueue");
		}
		LOG_NEWLINE("PEventQueue::pthread_mutex_init(result)", result);
	}
	virtual ~PEventQueue() 
	{
		sem_destroy(&m_semaphoreEmpty);
		sem_destroy(&m_semaphoreFull);
		pthread_mutex_destroy(&m_mutex);
	}

	void Initialize() override {
		EventQueue::Initialize();
	}
	void Finalize() override {
		EventQueue::Finalize();        
	}

	void PushBack(Event* pEvent) override {
//		sem_wait(&m_semaphoreFull);
		Lock();
		EventQueue::PushBack(pEvent);
		UnLock();
//		sem_post(&m_semaphoreEmpty);
	}
	Event* PopFront() override {
//		sem_wait(&m_semaphoreEmpty);
		Lock();
		Event *pEvent = EventQueue::PopFront();
		UnLock();
//		sem_post(&m_semaphoreFull);
		return pEvent;
	}
};
