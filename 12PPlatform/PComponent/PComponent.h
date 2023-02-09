#pragma once

#include <12PPlatform/typedef.h>
#define _PCompnent_Id _GET_CLASS_UID(_EPPlatform::_ePComponent)
#define _PCompnent_Name "PCompnent"

#include <02Platform/Component/Component.h>
#include <pthread.h>

#define PRIORITY 100

class PComponent : public Component
{
private:
	pthread_t m_thread;
	int m_idThared;
	size_t m_stsThread;

protected:
	void RegisterEventTypes();
	void RegisterExceptions();

	virtual void Initialize();
	virtual void Finalize();

public:
	PComponent(int nClassId = _PCompnent_Id, const char* pcClassName = _PCompnent_Name);
	virtual ~PComponent() override;

	virtual void Start();
	virtual void Stop();
	virtual void Run() {}
};