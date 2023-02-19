#pragma once

#include <09Application/typedef.h>
#define _Main_Id _GET_CLASS_UID(_ELayer_Application::_eMain)
#define _Main_Name "Main"

class IMain {
public:
	enum class EEventType {
		eBegin = _Main_Id,
		eInitializeAsAMain,
		eRunAsAMain,
		eFinalizeAsAMain,
		eEnd
	};
};