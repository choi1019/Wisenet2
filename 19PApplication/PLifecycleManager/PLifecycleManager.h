#pragma once

#include <19PApplication/typedef.h>
#define _PLifecycleManager_Id _GET_CLASS_UID(_ELayer_PApplication::_ePLifecycleManager)
#define _PLifecycleManager_name "PLifecycleManager"

#include <09Application/LifecycleManager/LifecycleManager.h>

class PLifecycleManager : public LifecycleManager {
public:
	enum class EComponents {
		eBegin = _PLifecycleManager_Id,
		eScheduler1,
		eScheduler2,
		eScheduler3,
		eTimerLinux,
		eTimerLinux1,
		eTimerLinux2,
		eTimerRTC,
		
		eVideoManager,
		eVideoRequesterManager,
		eVideoProviderManager,

		eStub,
		eSkeleton,

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
