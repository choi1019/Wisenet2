#pragma once

#include <02Platform/typedef.h>
#define _Scheduler_Id _GET_CLASS_UID(_ELayer_Platform::_eScheduler)
#define _Scheduler_Name "Scheduler"

#include <01Base/Object/ValueObject.h>
#include <02Platform/Component/Component.h>


class IScheduler
{
public:
	/// <summary>
	/// Required Interface
	/// </summary>
	enum class EEventType {
		eBegin = _Scheduler_Id,
		// create a thread
		eInitializeAsAScheduler,
		eFinalizeAsAScheduler,

		eAllocateAComponent,
		eDellocateAComponent,

		eStart,
		eIsStarted,
		ePause,
		eResume,
		eStop,
		eEnd
	};

	enum class EState {
		eBegin,
		eCreated,
		eInitializedAsAScheduler,
		eStarted,
		ePaused,
		eStopped,
		eFinalizedAsAScheduler,
		eEnd
	};

	enum class EError {
		eBegin = _Scheduler_Id,
		// create a thread
		eComponentNotFound,
		eNullPoint,
		eThreadCreationFailed,
		eThreadTerminationFailed,
		eEnd

	};

	class ParamAllocateAComponent : public ValueObject {
	private:
		Component* m_pComponentAllocated;
	public:
		ParamAllocateAComponent(Component* pComponentAllocated = nullptr)
			: m_pComponentAllocated(pComponentAllocated)
		{
		}
		~ParamAllocateAComponent() {
		}
		Component* GetPComponentAllocated() { return this->m_pComponentAllocated; }
		void SetPComponentAllocated(Component* pComponentAllocated) { this->m_pComponentAllocated = pComponentAllocated; }
	};
};

