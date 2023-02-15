#include <13PTechnical/PTimer/PTimerLinux.h>

int PTimerLinux::s_counterId = 0;
// timer signal call back
void SignalPTimerLinux(int sig, siginfo_t *si, void *uc) {
    //PTimerLinux *pPTimerLinux = (PTimerLinux *) si->_sifields._rt.si_sigval.sival_ptr;
}
// thread callback
void* CallBackPTimerLinux(void *pObject) {
    PTimerLinux *pPTimerLinux = (PTimerLinux *)pObject;
    pPTimerLinux->RunThread();
    return nullptr;
}

// PTimer
PTimerLinux::PTimerLinux(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;

    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);

    m_msecInterval = msecInterval % 1000;
    m_secInterval = msecInterval / 1000;

    // Register signal action - bitwise OR with default actions
    m_sigAction.sa_flags = SA_SIGINFO;
    sigemptyset(&(m_sigAction.sa_mask));
    m_sigAction.sa_sigaction = SignalPTimerLinux;
    result = sigaction(SIGRTMIN, &m_sigAction, NULL);
    if ( result == -1){
        throw Exception((int)ITimer::EException::eInvalidEvent, "PTimerLinux::PTimerLinux", result);
    }

    // sigevent - how the caller should be notified when the timer expires.
    m_sigEvent.sigev_notify = SIGEV_SIGNAL; // Linux-specific
    m_sigEvent.sigev_signo = SIGRTMIN;
    m_sigEvent.sigev_value.sival_ptr = this;
    // create a timer
    result = timer_create(CLOCK_REALTIME, &m_sigEvent, &m_idTimer);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux::PTimerLinux", result);
    }
}

PTimerLinux::~PTimerLinux() {
    timer_delete(m_idTimer);
 	pthread_mutex_destroy(&m_mutex);
}

void PTimerLinux::Initialize() {
    Timer::Initialize();
}

void PTimerLinux::Finalize() {
    Timer::Finalize();

}

void PTimerLinux::Start() {
    Timer::Start();
    PThread::Fork(CallBackPTimerLinux, this);
}

// Thread
void PTimerLinux::RunThread() {
    // start the timer
    m_iTimerSpec.it_value.tv_sec  = 1;
    m_iTimerSpec.it_value.tv_nsec = 0;
    m_iTimerSpec.it_interval.tv_sec  = m_msecInterval;
    m_iTimerSpec.it_interval.tv_nsec = m_secInterval; 
    int result = timer_settime(m_idTimer, 0, &m_iTimerSpec, NULL);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eSetTimerError, "PTimerLinux::PTimerLinux", result);
    }
    // wait until unlock
    pthread_mutex_lock(&m_mutex);
}

void PTimerLinux::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

void PTimerLinux::Stop() {
    // Disable periodic interrupts
    pthread_mutex_unlock(&m_mutex);
    Timer::Stop();
    PThread::Join();
}

void PTimerLinux::TimeOut(Event *pEvent) {
//    SendTargetEvents((unsigned)ITimer::EGroups::eGroup1, (unsigned)ITimer::EEventType::eTimeOut);
    LOG_NEWLINE("%%%%", this->GetObjectId(), " PTimer::TimeOut()", m_uCounter++);
}

void PTimerLinux::ProcessAEvent(Event *pEvent) {
    switch(pEvent->GetType()) {
        case (unsigned)ITimer::EEventType::eTimeOut:
            this->TimeOut(pEvent);
            break;
        default:
        	Timer::ProcessAEvent(pEvent);
            break;
    }

}