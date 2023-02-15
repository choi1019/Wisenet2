
#include <03Technical/typedef.h>
#define _Timer_Id _GET_CLASS_UID(_ELayer_Technical::_eTimer)
#define _Timer_Name "Timer"

class ITimer {
public:
	////////////////////////////////////////
	// Component Parts
	////////////////////////////////////////
	enum class EPart {
		eBegin = _Timer_Id,
		eTimerPart,
		eEnd
	};

	////////////////////////////////////////
	// provided interface
	////////////////////////////////////////
	enum class EGroups {
		eBegin = _Timer_Id,
		eGroup1,
		eGroup2,
		eEnd
	};

	// event type
	enum class EEventType {
		eBegin = _Timer_Id,
		eTimeOut,
		eEnd
	};
    
	enum class EException {
		eBegin = _Timer_Id,
		eTimerCreationError,
		eInvalidEvent,
		eInvalidHandler,
		eSetTimerError,
		eEnd
	};
};