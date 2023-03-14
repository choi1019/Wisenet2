#pragma once

#include <12PPlatform/typedef.h>
#define PComponentPart_Id _GET_CLASS_UID(_EPPlatform::_ePComponentPart)
#define PComponentPart_Name "PComponentPart"

#include <02Platform/Component/ComponentPart.h>
#include <13PTechnical/PThread/PThread.h>

#define PRIORITY 100

class PComponentPart : public ComponentPart, public PThread {

public:
	// for Component
	PComponentPart(int uClassId = PComponentPart_Id, const char* acClassName = PComponentPart_Name);
	virtual ~PComponentPart();

	void Fork();
	void Join();
	virtual void RunThread();

	void Start();
    void Stop();
};