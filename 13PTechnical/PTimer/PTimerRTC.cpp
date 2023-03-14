#include <13PTechnical/PTimer/PTimerRTC.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

void* CallBackPTimerRTC(void *pObject) {
	PTimerRTC *pPTimer = (PTimerRTC *)pObject;
	pPTimer->RunThread();
	return nullptr;
}

// PTimer
PTimerRTC::PTimerRTC(size_t szPeriod, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_szPeriod(szPeriod)
    , m_uCounter(0)
{
    char path[20] = "/dev/rtc";
    m_nFd = open(path, O_RDONLY);
    if (m_nFd < 0) {
 	}
}

PTimerRTC::~PTimerRTC() {
    close(m_nFd);
}

void PTimerRTC::Initialize() {
    Timer::Initialize();

    int retval = ioctl(m_nFd, RTC_IRQP_SET, 2);
	if (retval == -1) {
	}
}

void PTimerRTC::Finalize() {
    Timer::Finalize();

}

void PTimerRTC::Start() {
    this->SetEState(IComponent::EState::eRunning);
    PThread::Fork(&CallBackPTimerRTC, this);
    // Enable periodic interrupts
//	int retval = ioctl(m_nFd, RTC_UIE_ON, 0);
    int retval = ioctl(m_nFd, RTC_PIE_ON, 0);
	if (retval == -1) {
	}
}

// Thread
void PTimerRTC::RunThread() {
    size_t counter = 0;
    while(this->GetEState() == IComponent::EState::eRunning) {
        // This blocks
        size_t data;
        int retval = read(m_nFd, &data, sizeof(unsigned long));
        if (retval == -1) {
            throw Exception((int)ITimer::EException::eInvalidEvent, "PTimerRTC::RunThread()");
        }
        SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
    }
}

void PTimerRTC::Stop() {
    // Disable periodic interrupts
	int retval = ioctl(m_nFd, RTC_PIE_OFF, 0);
//    int retval = ioctl(m_nFd, RTC_UIE_OFF, 0);
	if (retval == -1) {
	}
    this->SetEState(IComponent::EState::eStopped);
    PThread::Join();
}

void PTimerRTC::TimeOut(Event *pEvent) {
//    SendTargetEvents((unsigned)ITimer::EGroups::eGroup1, (unsigned)ITimer::EEventType::eTimeOut);
    LOG_NEWLINE("%%%%", this->GetComponentId(), " PTimer::TimeOut()", m_uCounter++);
}

void PTimerRTC::ProcessAEvent(Event *pEvent) {
    switch(pEvent->GetType()) {
        case (unsigned)ITimer::EEventType::eTimeOut:
            this->TimeOut(pEvent);
            break;
        default:
            Timer::ProcessAEvent(pEvent);
            break;
    }

}