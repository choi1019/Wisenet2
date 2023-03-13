#pragma once

#include <19PApplication/typedef.h>
#define _PLifecycleManager_Id _GET_CLASS_UID(_ELayer_PApplication::_ePLifecycleManager)
#define _PLifecycleManager_Name "PLifecycleManager"

#include <01Base/Object/ValueObject.h>
#include <02Platform/Scheduler/Scheduler.h>

class IPLifecycleManager {
public:
public:
	enum class EComponents {
		eBegin = _PLifecycleManager_Id,
		ePScheduler0,
		ePScheduler1,
		ePScheduler2,
		eScheduler3,
		eTimerLinux,
		eTimerLinux1,
		eTimerLinux2,
		eTimerRTC,
		
		eVideoManager,
		eVideoRequesterManager,
		eVideoProviderManager,

		ePStub,
		ePSkeleton,

		eEnd
	};
};
