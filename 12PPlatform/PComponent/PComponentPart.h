#pragma once

#include <12PPlatform/typedef.h>
#define PComponentPart_Id _GET_CLASS_UID(_EPPlatform::_ePComponentPart)
#define PComponentPart_Name "PComponentPart"  // class string name

#include <02Platform/Component/ComponentPart.h>
#include <pthread.h>

#define PRIORITY 100

class PComponentPart : public ComponentPart {

private:
	pthread_t m_thread;
	int m_idThared;
	size_t m_stsThread;

public:
	// for Component
	PComponentPart(int uClassId = PComponentPart_Id, const char* acClassName = PComponentPart_Name);
	virtual ~PComponentPart();
	virtual void Initialize();
	virtual void Finalize();

	virtual void Start();
	virtual void Stop();
	virtual void Run() {}
};