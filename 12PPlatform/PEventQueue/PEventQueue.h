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

class Scheduler;

class PEventQueue: public EventQueue {
private:
	pthread_mutex_t m_mutex;
	sem_t m_semaphoreFull;
	sem_t m_semaphoreEmpty;

protected:
	void PushLock() override {
		sem_wait(&m_semaphoreFull);
        pthread_mutex_lock(&m_mutex);
	}
	void PushUnlock() override {
        pthread_mutex_unlock(&m_mutex);
		sem_post(&m_semaphoreEmpty);
	}
	void PopLock() override {
		sem_wait(&m_semaphoreEmpty);
        pthread_mutex_lock(&m_mutex);
	}
	void PopUnlock() override {
        pthread_mutex_unlock(&m_mutex);
		sem_post(&m_semaphoreFull);
	}
public:
	PEventQueue(
		Scheduler *pScheduler = nullptr,
		int nClassId = _PEventQueue_Id, 
		const char* pcClassName = _PEventQueue_Name);
	virtual ~PEventQueue();

	void Initialize() override;
	void Finalize() override;
};
