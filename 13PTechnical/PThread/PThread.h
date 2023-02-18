#pragma once

#include <13PTechnical/typedef.h>
#define _PThread_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePThread)
#define _PThread_Name "PThread"

#include <pthread.h>
class PThread
{
private:
    pthread_t m_thread;
    int m_idThared;
    int m_stsThread;
public:
	PThread(int nComponentId = _PThread_Id, const char* sComponentName = _PThread_Name);

	virtual ~PThread();

	virtual void Fork(void *(*__start_routine)(void *), void *pObject);
	virtual void Join();
    virtual void RunThread() = 0;
};