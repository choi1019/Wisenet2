#include <13PTechnical/PTimer/PTimerLinux1.h>

int PTimerLinux1::s_counterId = 0;
PTimerLinux1 *PTimerLinux1::s_pPTimerLinux1 = nullptr;

// thread callback
static void* CallBackPTimerLinux1(void *pObject) {
    PTimerLinux1 *pPTimerLinux1 = (PTimerLinux1 *)pObject;
    pPTimerLinux1->RunThread();
    return nullptr;
}

// timer signal call back
static void SignalPTimerLinux1(int noSig) {
    PTimerLinux1::s_pPTimerLinux1->Signal();
}

// PTimer
PTimerLinux1::PTimerLinux1(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;
    PTimerLinux1::s_pPTimerLinux1 = this;

    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);

    m_secInterval = msecInterval / 1000;
    m_msecInterval = msecInterval % 1000;
}

PTimerLinux1::~PTimerLinux1() {
    timer_delete(m_idTimer);
 	pthread_mutex_destroy(&m_mutex);
}

void PTimerLinux1::Initialize() {
    Timer::Initialize();
}

void PTimerLinux1::Finalize() {
    Timer::Finalize();

}

void PTimerLinux1::Start() {
    Timer::Start();
    PThread::Fork(CallBackPTimerLinux1, this);
}

// Thread
void PTimerLinux1::RunThread() {
    struct sigaction sa;
    struct itimerval timer;

    /* Install timer_handler as the signal handler for SIGVTALRM. */
    memset (&sa, 0, sizeof (sa));
    sa.sa_handler = SignalPTimerLinux1;
    int result = sigaction (SIGVTALRM, &sa, NULL);
    if (result == -1)  
    {  
        throw Exception((int)ITimer::EException::eInvalidEvent);
    } 
    /* Configure the timer to expire after 250 msec... */
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1;
    timer.it_interval.tv_sec = m_secInterval;
    timer.it_interval.tv_usec = m_msecInterval * 1000;

    /* Start a virtual timer. It counts down whenever this process is executing. */
    result = setitimer (ITIMER_VIRTUAL, &timer, NULL);
    if (result == -1)  
    {  
        throw Exception((int)ITimer::EException::eInvalidEvent);
    } 
     // wait until unlock
     while(1);
    pthread_mutex_lock(&m_mutex);
}

void PTimerLinux1::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

void PTimerLinux1::Stop() {
    
    // Disable periodic interrupts
    pthread_mutex_unlock(&m_mutex);
    Timer::Stop();
    PThread::Join();
}

void PTimerLinux1::TimeOut(Event *pEvent) {
//    SendTargetEvents((unsigned)ITimer::EGroups::eGroup1, (unsigned)ITimer::EEventType::eTimeOut);
    LOG_NEWLINE("%%%%", this->GetObjectId(), " PTimer::TimeOut()", m_uCounter++);
}

void PTimerLinux1::ProcessAEvent(Event *pEvent) {
    switch(pEvent->GetType()) {
        case (unsigned)ITimer::EEventType::eTimeOut:
            this->TimeOut(pEvent);
            break;
        default:
        	Timer::ProcessAEvent(pEvent);
            break;
    }

}