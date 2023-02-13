#pragma once
#include <pthread.h>

class PThread
{
private:
    pthread_t m_thread;
    int m_idThared;
    int m_stsThread;
public:
	PThread();

	virtual ~PThread();

	virtual void Fork(void *(*__start_routine)(void *), void *pObject);
	virtual void Join();
    virtual void RunThread() = 0;
};