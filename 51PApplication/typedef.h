#pragma once

#include <typedef.h>

enum class _EPApplication {
	_eBegin = _GET_LAYER_UID(_ELayer::_ePApplication),

	_ePLifecycleManager,
	_ePMain,

	_eEnd
};