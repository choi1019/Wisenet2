#include <13PTechnical/PTimer/PTimer.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

void* CallBackPTimer(void *pObject) {
	PTimer *pPTimer = (PTimer *)pObject;
	pPTimer->RunThread();
	return nullptr;
}

// PTimer
PTimer::PTimer(size_t szPeriod, int nComponentId, const char* sComponentName) 
    : Timer(nComponentId, sComponentName) 
    , m_szPeriod(szPeriod)
    , m_uCounter(0)
{
    char path[20] = "rtc";
    m_nFd = open(path, O_RDONLY);
    if (m_nFd < 0) {
 	}
}

PTimer::~PTimer() {
    close(m_nFd);
}

void PTimer::Initialize() {
    Timer::Initialize();

    int retval = ioctl(m_nFd, RTC_IRQP_SET, 0);
	if (retval == -1) {
	}
}

void PTimer::Finalize() {
    Timer::Finalize();

}

void PTimer::Start() {
    Timer::Start();
    PThread::Fork();
    // Enable periodic interrupts
	int retval = ioctl(m_nFd, RTC_PIE_ON, 0);
	if (retval == -1) {
	}
}

// Thread
void PTimer::RunThread() {
    size_t counter = 0;
    while(this->GetEState() == Component::EState::eRunning) {
        // This blocks
        size_t data;
        int retval = read(m_nFd, &data, sizeof(unsigned long));
        if (retval == -1) {
        }
//       if (++counter > m_szPeriod) {
            SendNoReplyEvent(this->GetUId(), (unsigned)ITimer::EEventType::eTimeOut);
//          counter = 0;
//        }
    }
}

void PTimer::Stop() {
    // Disable periodic interrupts
	int retval = ioctl(m_nFd, RTC_PIE_OFF, 0);
	if (retval == -1) {
	}
    Timer::Stop();
    PThread::Join();
}

void PTimer::TimeOut(Event *pEvent) {
//    SendTargetEvents((unsigned)ITimer::EGroups::eGroup1, (unsigned)ITimer::EEventType::eTimeOut);
    LOG_NEWLINE("%%%% PTimer::TimeOut()", m_uCounter++);
}

void PTimer::ProcessAEvent(Event *pEvent) {
    switch(pEvent->GetType()) {
        case (unsigned)ITimer::EEventType::eTimeOut:
            this->TimeOut(pEvent);
            break;
        default:
            break;
    }

}