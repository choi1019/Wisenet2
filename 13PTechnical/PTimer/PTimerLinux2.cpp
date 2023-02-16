#include <13PTechnical/PTimer/PTimerLinux2.h>

int PTimerLinux2::s_counterId = 0;

// thread callback
static void* CallBackPTimerLinux2(void *pObject) {
    PTimerLinux2 *pPTimerLinux = (PTimerLinux2 *)pObject;
    pPTimerLinux->RunThread();
    return nullptr;
}
// timer signal call back
static void SignalPTimerLinux2(int sig, siginfo_t *si, void *uc) {
    PTimerLinux2 *pPTimerLinux2 = (PTimerLinux2 *) si->si_value.sival_ptr;
}

// Thread
void PTimerLinux2::RunThread() {	
    timer_t *m_pTimer;
    m_pTimer = new timer_t();

    // Register signal action - bitwise OR with default actions
    struct sigaction m_sigAction;
    m_sigAction.sa_flags = SA_SIGINFO; 
    m_sigAction.sa_sigaction = &SignalPTimerLinux2; 
    sigemptyset(&m_sigAction.sa_mask);
    int result = sigaction(SIGRTMIN, &m_sigAction, NULL);
    if ( result == -1){
        throw Exception((int)ITimer::EException::eInvalidEvent, "PTimerLinux2::PTimerLinux2", result);
    }

    // Block timer signal temporarily.
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGRTMIN);
    result = sigprocmask(SIG_SETMASK, &mask, NULL) ;
    if (result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux2::PTimerLinux2", result);
    }

    // sigevent - how the caller should be notified when the timer expires.
    struct sigevent m_sigEvent;
    m_sigEvent.sigev_notify = SIGEV_SIGNAL; // Linux-specific
    m_sigEvent.sigev_signo = SIGRTMIN;
    m_sigEvent.sigev_value.sival_ptr = m_pTimer;
    // create a timer
    result = timer_create(CLOCK_REALTIME, &m_sigEvent, m_pTimer);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux2::PTimerLinux2", result);
    }

    // start the timer
    struct itimerspec m_intervalTimerSpec;
    m_intervalTimerSpec.it_value.tv_sec  = m_secInterval;
    m_intervalTimerSpec.it_value.tv_nsec = m_msecInterval * 1000000;
    m_intervalTimerSpec.it_interval.tv_sec  = m_secInterval;
    m_intervalTimerSpec.it_interval.tv_nsec = m_msecInterval * 1000000; 
    result = timer_settime(*m_pTimer, 0, &m_intervalTimerSpec, NULL);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eSetTimerError, "PTimerLinux2::PTimerLinux2", result);
    }
    sleep(2);
    /* Unlock the timer signal, so that timer notification can be delivered. */
    result = sigprocmask(SIG_UNBLOCK, &mask, NULL) ;
    if (result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux2::PTimerLinux2", result);
    }
    // wait until unlock
    pthread_mutex_lock(&m_mutex);
}

void PTimerLinux2::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

////////////////////////////////////////////////////////////////
// PTimer
PTimerLinux2::PTimerLinux2(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;
    m_msecInterval = msecInterval % 1000;
    m_secInterval = msecInterval / 1000;

 

    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);


}

PTimerLinux2::~PTimerLinux2() {
 	pthread_mutex_destroy(&m_mutex);
}

void PTimerLinux2::Initialize() {
    Timer::Initialize();
}

void PTimerLinux2::Finalize() {
    Timer::Finalize();

}

void PTimerLinux2::Start() {
    Timer::Start();
    PThread::Fork(CallBackPTimerLinux2, this);
}

void PTimerLinux2::Stop() {
    // Disable periodic interrupts
    pthread_mutex_unlock(&m_mutex);
    Timer::Stop();
    PThread::Join();
}

void PTimerLinux2::TimeOut(Event *pEvent) {
//    SendTargetEvents((unsigned)ITimer::EGroups::eGroup1, (unsigned)ITimer::EEventType::eTimeOut);
    LOG_NEWLINE("%%%%", this->GetObjectId(), " PTimer::TimeOut()", m_uCounter++);
}

void PTimerLinux2::ProcessAEvent(Event *pEvent) {
    switch(pEvent->GetType()) {
        case (unsigned)ITimer::EEventType::eTimeOut:
            this->TimeOut(pEvent);
            break;
        default:
        	Timer::ProcessAEvent(pEvent);
            break;
    }

}