#include <13PTechnical/PTimer/PTimerLinux.h>
#include <signal.h>
#include <sys/time.h>

PTimerLinux *PTimerLinux::s_apPTimer[TIMER_NUMMAX] = {};
int PTimerLinux::s_counterId = 0;

//////////////////////////////////////////////
// timer signal call back
void SignalPTimerLinux0(int signum) {
	PTimerLinux::s_apPTimer[0]->Signal();
}
void SignalPTimerLinux1(int signum) {
	PTimerLinux::s_apPTimer[1]->Signal();
}
void SignalPTimerLinux2(int signum) {
	PTimerLinux::s_apPTimer[2]->Signal();
}
void SignalPTimerLinux3(int signum) {
	PTimerLinux::s_apPTimer[3]->Signal();
}
void(*PTimerLinux::CallbackSignal[TIMER_NUMMAX])(int) = {
    &SignalPTimerLinux0,
    &SignalPTimerLinux1,
    &SignalPTimerLinux2,
    &SignalPTimerLinux3
};
//////////////////////////////////////////////

//////////////////////////////////////////////
// thread callback
void* CallBackPTimerLinux(void *pObject) {
    PTimerLinux *pPTimerLinux = (PTimerLinux *)pObject;
    pPTimerLinux->RunThread();
    return nullptr;
}
//////////////////////////////////////////////

// PTimer
PTimerLinux::PTimerLinux(size_t msecInterval, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_uCounter(0)
{
    m_nId = s_counterId++;
    PTimerLinux::s_apPTimer[m_nId] = this;

    int result = pthread_mutex_init(&m_mutex, nullptr);
    pthread_mutex_lock(&m_mutex);

    m_msecInterval = msecInterval % 1000;
    m_secInterval = msecInterval / 1000;
}

PTimerLinux::~PTimerLinux() {
    pthread_mutex_unlock(&m_mutex);
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
    PThread::Fork(&CallBackPTimerLinux, this);
}

// Thread
void PTimerLinux::RunThread() {
    struct itimerval m_timeNew;
	struct itimerval m_timeOld;
    m_timeNew.it_value.tv_sec = 0;
    m_timeNew.it_value.tv_usec = 0;
    m_timeNew.it_interval.tv_sec = m_secInterval;
    m_timeNew.it_interval.tv_usec = m_msecInterval * 1000;
    setitimer(ITIMER_REAL, &m_timeNew, &m_timeOld);
    signal(SIGALRM, PTimerLinux::CallbackSignal[m_nId]);
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