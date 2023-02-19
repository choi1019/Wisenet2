#pragma once

#include <typedef.h>

enum class _ELayer_PTechnical {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePTechnical),

	_ePMemoryStatic,
	_ePMemoryDynamic,
	_ePMemoryEven,
	_ePMemoryManager,

	_ePThread,
	
	_ePTimerRTC,
	_ePTimerLinux,
	_ePTimerLinux1,
	_ePTimerLinux2,

	_ePStub,
	_ePSkeleton,
	_ePSkeletonWorker,

	_eEnd
};


