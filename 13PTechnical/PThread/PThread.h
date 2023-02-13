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

	virtual void Fork();
	virtual void Join();
    virtual void RunThread() = 0;
};