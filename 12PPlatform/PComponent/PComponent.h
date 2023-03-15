#pragma once

#include <12PPlatform/typedef.h>
#define _PCompnent_Id _GET_CLASS_UID(_EPPlatform::_ePComponent)
#define _PCompnent_Name "PCompnent"

#include <02Platform/Component/Component.h>
#include <13PTechnical/PThread/PThread.h>
#define PRIORITY 100

class PComponent : public Component, public PThread
{
private:
	pthread_mutex_t m_mutex;
protected:
	void RegisterEventTypes();
	void RegisterExceptions();

public:
	PComponent(int nClassId = _PCompnent_Id, const char* pcClassName = _PCompnent_Name);
	virtual ~PComponent() override;

	IComponent::EState GetEState() override;
	void SetEState(IComponent::EState eState) override;

	void Fork();
	void Join();
	virtual void RunThread() = 0;

	virtual void Start();
	virtual void Stop();


};