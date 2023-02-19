#pragma once

#include <19PApplication/typedef.h>
#define _PMain_Id _GET_CLASS_UID(_ELayer_PApplication::_ePMain)
#define _PMain_Name "PMain"

#include <09Application/Main/Main.h>

class PMain : public Main
{
public:
	static int main_ex();

public:
	PMain(unsigned uClassId = _PMain_Id,
		const char* pcClassName = _PMain_Name);
	virtual ~PMain();

	// as a Component
	void Initialize() override;
	void Finalize() override;
	
	void BootstrapSystem() override;
	void RunAsAMain() override;
	void ShutdownSystem() override;
};