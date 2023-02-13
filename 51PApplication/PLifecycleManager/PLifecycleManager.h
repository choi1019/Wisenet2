#pragma once

#include <51PApplication/typedef.h>
#define _PLifecycleManager_Id _GET_CLASS_UID(_EPApplication::_ePLifecycleManager)
#define _PLifecycleManager_name "PLifecycleManager"

#include <02Platform/LifecycleManager/LifecycleManager.h>

class PLifecycleManager : public LifecycleManager {
public:
	enum class EComponents {
		eBegin = _PLifecycleManager_Id,
		eScheduler1,
		eScheduler2,
		eScheduler3,
		eTimer,

		eVideoManager,
		eVideoInput,

		eEnd
	};
public:
	PLifecycleManager(
		unsigned typeId = _PLifecycleManager_Id,
		const char* pClassName = _PLifecycleManager_name);
	virtual ~PLifecycleManager();

protected:
	virtual void RegisterUserShedulers();
	virtual void RegisterUserComponents();
	virtual void AllocateUserComponents();
	virtual void AssociateUserSendersNReceivers();
	virtual void AssociateUserSourcesNTargets();

	virtual void Initialize();
	virtual void Finalize();

	virtual void StartComponents();
	virtual void StopComponents();
};
