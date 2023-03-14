#include <13PTechnical/PTimer/PTimerLinux.h>
#include <signal.h>
#include <sys/time.h>

// static variables
int PTimerLinux::s_counterId = 0;
PTimerLinux *PTimerLinux::s_pPTimerLinux = nullptr;

// thread callback
static void* CallBackPTimerLinux(void *pObject) {
    PTimerLinux *pPTimerLinux = (PTimerLinux *)pObject;
    pPTimerLinux->RunThread();
    return nullptr;
}
// signal call back
static void SignalPTimerLinux(int noSig) {
    PTimerLinux::s_pPTimerLinux->Signal();
}

// Thread Method
void PTimerLinux::RunThread() {
    struct itimerval intervalTimerValue;
    intervalTimerValue.it_value.tv_sec = 1;
    intervalTimerValue.it_value.tv_usec = 0;
    intervalTimerValue.it_interval.tv_sec = m_secInterval;
    intervalTimerValue.it_interval.tv_usec = m_msecInterval * 1000;

    /* Start a virtual timer. It counts down whenever this process is executing. */
    int result = setitimer (ITIMER_REAL, &intervalTimerValue, nullptr);
    if (result == -1) {  
        throw Exception((int)ITimer::EException::eInvalidEvent);
    } 
    signal(SIGALRM, SignalPTimerLinux);

    // wait until unlock
    pthread_mutex_lock(&m_mutex);
}
// signal method
void PTimerLinux::Signal() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

////////////////////////////////////////////////////////////////////////////////////
// component methods
PTimerLinux::PTimerLinux(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;
    m_secInterval = msecInterval / 1000;
    m_msecInterval = msecInterval % 1000;

    PTimerLinux::s_pPTimerLinux = this;
    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);
}
PTimerLinux::~PTimerLinux() {
 	pthread_mutex_destroy(&m_mutex);
}

void PTimerLinux::Start() {
    pthread_mutex_lock(&m_mutex);
    this->SetEState(IComponent::EState::eRunning);
    pthread_mutex_unlock(&m_mutex);
    PThread::Fork(&CallBackPTimerLinux, this);
}
void PTimerLinux::Stop() {    
    // Disable periodic interrupts
    pthread_mutex_lock(&m_mutex);
    this->SetEState(IComponent::EState::eStopped);
    pthread_mutex_unlock(&m_mutex);
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