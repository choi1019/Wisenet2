#pragma once

#include <01Base/typedef.h>
#define _Directory_Id _GET_CLASS_UID(_ELayer_Base::_eDirectory)
#define _Directory_Name "Exception"

#include <01Base/StdLib/Map.h>
#include <01Base/StdLib/String.h>

typedef Map<size_t, String> DirectoryMap;
class Directory : public DirectoryMap
{
public:
	enum class EType {
		eObject,
		eComponent,
		eScheduler,
		eEvent,
		eException,
		eLog,
		eEnd
	};

	static void ClearDirectories();
	
private:
	EType m_eType;

public:
	static Directory s_dirObjects;
	static Directory s_dirComponents;
	static Directory s_dirEvents;
	static Directory s_dirExceptions;

	Directory(EType eType)
		: DirectoryMap(_Directory_Id, _Directory_Name)
		, m_eType(eType)
	{
	}
	virtual ~Directory() {}

	virtual void Initialize() {
		DirectoryMap::Initialize();
	}
	virtual void Finalize() {
		DirectoryMap::Finalize();
	}
	void Show(const char* pcTitle);
};

#define REGISTER_EVENT(COMPONENTNAME, EVENTTYPE) Directory::s_dirEvents[(int)COMPONENTNAME::EEventType::EVENTTYPE] = "EVENTTYPE"
#define SHOW_EVENTNAME(EVENTTYPE) Directory::s_dirEvents[EVENTTYPE] + EVENTTYPE
#define SHOW_COMPONENTNAME(COMPONENTID) Directory::s_dirComponents[COMPONENTID] + COMPONENTID
//Directory::s_dirComponents[pEvent->GetUIdTarget().GetComponentId()] + pEvent->GetUIdTarget().GetComponentId()



