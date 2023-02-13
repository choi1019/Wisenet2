#include <13PTechnical/PTimer/PTimerLinux.h>
#include <signal.h>

PTimerLinux *PTimerLinux::s_pPTimer = nullptr;
pthread_mutex_t *PTimerLinux::s_pMutex;

void SignalPTimerLinux(int signum) {
	PTimerLinux::s_pPTimer->RunThread();
    LOG_NEWLINE("SignalPTimerLinux(signum)", signum);
}

void* CallBackPTimerLinux(void *pObject) {
    struct itimerval m_timeNew;
	struct itimerval m_timeOld;
    m_timeNew.it_value.tv_sec = 1;
    m_timeNew.it_value.tv_usec = 0;
    m_timeNew.it_interval.tv_sec = 0;
    m_timeNew.it_interval.tv_usec = 300 * 1000;
    setitimer(ITIMER_REAL, &m_timeNew, &m_timeOld);
    signal(SIGALRM, SignalPTimerLinux);
    pthread_mutex_lock(PTimerLinux::s_pMutex);
    pthread_mutex_lock(PTimerLinux::s_pMutex);
    return nullptr;
}

// PTimer
PTimerLinux::PTimerLinux(size_t szPeriod, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_szPeriod(szPeriod)
    , m_uCounter(0)
{
    PTimerLinux::s_pPTimer = this;
    PTimerLinux::s_pMutex = new pthread_mutex_t();
    int result = pthread_mutex_init(PTimerLinux::s_pMutex, nullptr);
}

PTimerLinux::~PTimerLinux() {
 	pthread_mutex_destroy(PTimerLinux::s_pMutex);
}

void PTimerLinux::Initialize() {
    Timer::Initialize();
}

void PTimerLinux::Finalize() {
    Timer::Finalize();

}

void PTimerLinux::Start() {
    Timer::Start();
    PThread::Fork(&CallBackPTimerLinux, this);
}

// Thread
void PTimerLinux::RunThread() {
    SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
}

void PTimerLinux::Stop() {
    // Disable periodic interrupts
    pthread_mutex_unlock(PTimerLinux::s_pMutex);
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
            break;
    }

}