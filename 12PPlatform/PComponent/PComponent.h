#pragma once

#include <12PPlatform/typedef.h>
#define _PCompnent_Id _GET_CLASS_UID(_EPPlatform::_ePComponent)
#define _PCompnent_Name "PCompnent"

#include <02Platform/Component/Component.h>

#define PRIORITY 100

class PComponent : public Component
{
public:
	PComponent(int nClassId = _PCompnent_Id, const char* pcClassName = _PCompnent_Name);
	virtual ~PComponent();

	virtual void Run();
	virtual void Start();

protected:
	virtual void RegisterEventTypes();
	virtual void RegisterExceptions();

	virtual void Initialize() override;
	virtual void Finalize() override;

	virtual void Stop();
	virtual void Pause();
};