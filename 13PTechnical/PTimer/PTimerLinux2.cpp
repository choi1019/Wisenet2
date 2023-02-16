#include <13PTechnical/PTimer/PTimerLinux2.h>

int PTimerLinux2::s_counterId = 0;

// timer signal call back
static void SignalPTimerLinux2(int sig, siginfo_t *si, void *uc) {
    PTimerLinux2 *pPTimerLinux2 = (PTimerLinux2 *) si->si_value.sival_ptr;
}

// thread callback
static void* CallBackPTimerLinux2(void *pObject) {
    PTimerLinux2 *pPTimerLinux2 = (PTimerLinux2 *)pObject;
    pPTimerLinux2->RunThread();
    return nullptr;
}

// PTimer
PTimerLinux2::PTimerLinux2(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;
    m_pIdTimer = new timer_t();

    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);

    m_msecInterval = msecInterval % 1000;
    m_secInterval = msecInterval / 1000;
}

PTimerLinux2::~PTimerLinux2() {
    timer_delete(*m_pIdTimer);
    delete m_pIdTimer;
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

// Thread
void PTimerLinux2::RunThread() {
    // Register signal action - bitwise OR with default actions
    m_sigAction.sa_flags = SA_SIGINFO; 
    m_sigAction.sa_sigaction = SignalPTimerLinux2; 
    sigemptyset(&m_sigAction.sa_mask);
    int result = sigaction(SIGRTMIN, &m_sigAction, NULL);
    if ( result == -1){
        throw Exception((int)ITimer::EException::eInvalidEvent, "PTimerLinux2::PTimerLinux2", result);
    }

    // sigevent - how the caller should be notified when the timer expires.
    m_sigEvent.sigev_notify = SIGEV_SIGNAL; // Linux-specific
    m_sigEvent.sigev_signo = SIGRTMIN;
    m_sigEvent.sigev_value.sival_ptr = m_pIdTimer;
    // create a timer
    result = timer_create(CLOCK_REALTIME, &m_sigEvent, m_pIdTimer);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux2::PTimerLinux2", result);
    }
    // start the timer
    m_iTimerSpec.it_value.tv_sec  = 1;
    m_iTimerSpec.it_value.tv_nsec = 0;
    m_iTimerSpec.it_interval.tv_sec  = m_secInterval;
    m_iTimerSpec.it_interval.tv_nsec = m_msecInterval * 1000000; 
    result = timer_settime(*m_pIdTimer, 0, &m_iTimerSpec, NULL);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eSetTimerError, "PTimerLinux2::PTimerLinux2", result);
    }
    // wait until unlock
    pthread_mutex_lock(&m_mutex);
}

void PTimerLinux2::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
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