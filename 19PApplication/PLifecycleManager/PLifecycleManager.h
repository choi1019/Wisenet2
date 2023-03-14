#pragma once

#include <19PApplication/PLifecycleManager/IPLifecycleManager.h>
#include <09Application/LifecycleManager/LifecycleManager.h>

class PLifecycleManager : public LifecycleManager, public IPLifecycleManager {

public:
	PLifecycleManager(
		unsigned typeId = _PLifecycleManager_Id,
		const char* pClassName = _PLifecycleManager_Name);
	~PLifecycleManager() override;

protected:
	virtual void RegisterUserShedulers();
	virtual void RegisterUserComponents();
	virtual void AllocateUserComponents();
	virtual void AssociateUserSendersNReceivers();
	virtual void AssociateUserSourcesNTargets();

	virtual void InitializeComponents(Event* pEvent);
	virtual void StartSystem(Event *pEvent);
	virtual void StopSystem(Event *pEvent);
};
