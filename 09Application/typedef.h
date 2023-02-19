#pragma once

#include <typedef.h>

enum class _ELayer_Application {
	_eBegin = _GET_LAYER_UID(_ELayer::_eApplication),

	_eLifecycleManager,
	_eMain,

	_eEnd
};
