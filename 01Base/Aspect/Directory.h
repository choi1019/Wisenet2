#pragma once

#include <01Base/typedef.h>
#define _Directory_Id _GET_CLASS_UID(_ELayer_Base::_eDirectory)
#define _Directory_Name "Exception"

#include <01Base/StdLib/Map.h>
#include <01Base/StdLib/String.h>

typedef Map<size_t, String> DirectoryMap;
class Directory
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
	DirectoryMap m_mapDirectory;
	EType m_eType;

public:
	static Directory s_dirComponents;
	static Directory s_dirEvents;
	static Directory s_dirExceptions;

	Directory(EType eType)
		: m_eType(eType)
	{
	}
	virtual ~Directory() {
		m_mapDirectory.SetSize(0);
	}

	inline MapIterator<size_t, String> begin() { return m_mapDirectory.begin(); }
	inline MapIterator<size_t, String> end() {	return m_mapDirectory.end();	}
	String& operator[](size_t key) { return m_mapDirectory[key]; }
	void Clear() { m_mapDirectory.Clear(); }
	virtual void Initialize() {
		m_mapDirectory.Initialize();
	}
	virtual void Finalize() {
		m_mapDirectory.Finalize();
	}
	void Show(const char* pcTitle);
};

#define SHOW_COMPONENTNAME(COMPONENTID) Directory::s_dirComponents[COMPONENTID]

#define REGISTER_EVENT(COMPONENTNAME, EVENTTYPE) Directory::s_dirEvents[(int)COMPONENTNAME::EEventType::EVENTTYPE] = "EVENTTYPE"
#define SHOW_EVENTNAME(EVENTTYPE) Directory::s_dirEvents[EVENTTYPE]

#define REGISTER_EXCEPTION(COMPONENTNAME, EXCEPTIONTYPE) Directory::s_dirExceptions[(int)COMPONENTNAME::EExceptionType::EXCEPTIONTYPE] = "EXCEPTIONTYPE"
#define SHOW_EXCEPTION(EXCEPTIONTYPE) Directory::s_dirExceptions[EXCEPTIONTYPE]


//Directory::s_dirComponents[pEvent->GetUIdTarget().GetComponentId()] + pEvent->GetUIdTarget().GetComponentId()



