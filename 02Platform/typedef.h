#pragma once

#include <typedef.h>

enum class _ELayer_Platform {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePlatform),

	_eUId,
	_eEvent,
	_eEventQueue,
	_eComponentPart,
	_eComponent,
	_eComponentStub,
	_eScheduler,

	_eEnd
};

#define MAXTARGETCOMPONENTS 20
