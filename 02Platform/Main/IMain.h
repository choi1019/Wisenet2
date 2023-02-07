#pragma once

#include <02Platform/typedef.h>
#define _Main_Id _GET_CLASS_UID(_ELayer_Platform::_eMain)
#define _Main_Name "Main"

class IMain {
public:
	enum class EEventType {
		eBegin = _Main_Id,
		eInitializeAsAMain,
		eFinalizeAsAMain,
		eEnd
	};
};