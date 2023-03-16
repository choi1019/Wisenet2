#pragma once

#include <02Platform/Component/Component.h>
#include <13PTechnical/PRemote/IPStub.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <13PTechnical/PRemote/PStubWorker.h>

#define MAXLINE     1024

class PStub : public Component, public IPStub
{
private:
    in_addr_t m_inAddressIP;
    int m_nNumPort;

public:
	PStub(int nNumPort, const char* sAddressIP = nullptr, int nComponentId = _PStub_Id, const char* sComponentName = _PStub_Name) 
    : Component(nComponentId, sComponentName)
    , m_nNumPort(nNumPort)
    {
        if (sAddressIP == nullptr) {
            // local host ip
            m_inAddressIP = INADDR_ANY;
        } else {
            m_inAddressIP = inet_addr(sAddressIP);
        }
    }
	~PStub() override {

    }

    void Register(Event *pEvent) {
        PStubWorker *pPStubWorker = new("pPStubWorker") PStubWorker(m_nNumPort, m_inAddressIP);
        this->AddPart((int)IPStub::EParts::ePStubWorker, pPStubWorker);
    }

    void Send(Event *pEvent) {
        PStubWorker *pPStubWorker = (PStubWorker *)GetPart((int)IPStub::EParts::ePStubWorker);
        pPStubWorker->Start(pEvent);
    }

    void Stop(Event *pEvent) {
    }

    void ProcessAEvent(Event *pEvent) {
        switch(pEvent->GetType()) {
            case (int)IPStub::EEventType::eRegister:
                this->Register(pEvent);
                break;
            case (int)IPStub::EEventType::eSend:
                this->Send(pEvent);
                break;
            default:
                Component::ProcessAEvent(pEvent);
                break;
        }
    }
};