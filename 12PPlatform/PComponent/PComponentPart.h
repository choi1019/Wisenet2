#pragma once

#include <12PPlatform/typedef.h>
#define PComponentPart_Id _GET_CLASS_UID(_EPPlatform::_ePComponentPart)
#define PComponentPart_Name "PComponentPart"  // class string name

#include <02Platform/Component/ComponentPart.h>

#define PRIORITY 100

class PComponentPart : public ComponentPart {

protected:
	void RegisterExceptions();

public:
	// for Component
	PComponentPart(
		unsigned uClassId = PComponentPart_Id,
		const char* acClassName = PComponentPart_Name);
	~PComponentPart() override;
	void Initialize() override;
	void Finalize() override;

	void Start();
	void Stop();
	void Pause();

	void Run();
};