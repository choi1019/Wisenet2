#include <13PTechnical/PTimer/PTimerLinux2.h>

int PTimerLinux2::s_counterId = 0;


// timer signal call back
static void SignalActionPTimerLinux2(int idSig, siginfo_t *pSigInfo, void *pUc) {
    ((PTimerLinux2 *) pSigInfo->si_value.sival_ptr)->Signal();
}
void PTimerLinux2::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

// thread callback
static void* CallBackPTimerLinux2(void *pObject) {
    PTimerLinux2 *pPTimerLinux = (PTimerLinux2 *)pObject;
    pPTimerLinux->RunThread();
    return nullptr;
}
void PTimerLinux2::RunThread() {
    // int result = pthread_mutex_init(&s_mutexSignal, nullptr);

    // Register signal action - bitwise OR with default actions
        // struct sigaction {
        // void     (*sa_handler)(int);
        // void     (*sa_sigaction)(int, siginfo_t *, void *);
        // sigset_t   sa_mask;
        // int        sa_flags;
        // void     (*sa_restorer)(void);
        // };
    m_sigAction.sa_flags = SA_SIGINFO;
    m_sigAction.sa_sigaction = SignalActionPTimerLinux2; 
    sigemptyset(&m_sigAction.sa_mask);
    int result = sigaction(SIGALRM, &m_sigAction, NULL);
    if ( result == -1){
        throw Exception((int)ITimer::EException::eInvalidEvent, "PTimerLinux2::PTimerLinux2", result);
    }

    // sigevent - how the caller should be notified when the timer expires.
    m_sigEvent.sigev_notify = SIGEV_SIGNAL;
    m_sigEvent.sigev_signo = SIGALRM;
    m_sigEvent.sigev_value.sival_ptr = this;
    // create a timer
    result = timer_create(CLOCK_REALTIME, &m_sigEvent, &m_timer);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eInvalidHandler, "PTimerLinux2::PTimerLinux2", result);
    }

    // // start the timer
    // // If new_value->it_value specifies a nonzero value (i.e., either subfield is nonzero), 
    // // then timer_settime() arms (starts) the timer
    m_intervalTimerSpec.it_value.tv_sec  = 1;
    m_intervalTimerSpec.it_value.tv_nsec = 0;
    m_intervalTimerSpec.it_interval.tv_sec  = m_secInterval;
    m_intervalTimerSpec.it_interval.tv_nsec = m_msecInterval * 1000000; 
    result = timer_settime(m_timer, 0, &m_intervalTimerSpec, NULL);
    if ( result != 0) {
        throw Exception((int)ITimer::EException::eSetTimerError, "PTimerLinux2::PTimerLinux2", result);
    }

    // wait until unlock
    pthread_mutex_lock(&m_mutex);
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
    pthread_mutex_lock(&m_mutex);
    this->SetEState(IComponent::EState::eRunning);
    pthread_mutex_unlock(&m_mutex);
    PThread::Fork(CallBackPTimerLinux2, this);
}

void PTimerLinux2::Stop() {
    pthread_mutex_lock(&m_mutex);
    this->SetEState(IComponent::EState::eStopped);
    pthread_mutex_unlock(&m_mutex);
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