#pragma once

#define undefined -1

#define _GET_LAYER_UID(ID) (unsigned)ID*100
#define _GET_CLASS_UID(ID) (unsigned)ID*100

enum class _ELayer {
	_eBegin = 0,

	_eBase = 1,
	_ePlatform = 2,
	_eTechnical = 3,
	_eAspect = 4,
	_eApplication = 9,

	_ePPlatform = 12,
	_ePTechnical = 13,
	_ePAspect = 14,
	_ePApplication = 19,

	_eDomain = 21,
	_eCamera = 22,
	_eRecoreder = 23,

	_ePDomain = 31,
	_ePCamera = 32,
	_ePRecoreder = 33,

	_eEnd
};

// for size_t
#include <cstddef>
// for memcpy
#include <string.h>
// printf
#include <stdio.h>
// duplication
#undef GetClassName

#define _DEBUG_L 1
#define _DEBUG_M 1
#define _DEBUG_E 1
