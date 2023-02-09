#pragma once

#include <51PApplication/typedef.h>
#define _PMain_Id _GET_CLASS_UID(_EPApplication::_ePMain)
#define _PMain_Name "PMain"

#include <02Platform/Main/Main.h>

class PMain : public Main
{
public:
	static int main_ex();

public:
	PMain(unsigned uClassId = _PMain_Id,
		const char* pcClassName = _PMain_Name);
	virtual ~PMain();

	virtual void BootstrapSystem();
	virtual void ShutdownSystem();
};