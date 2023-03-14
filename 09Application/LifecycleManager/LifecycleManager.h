#pragma once

#include <01Base/StdLib/Map.h>
#include <01Base/StdLib/Vector.h>
#include <09Application/LifecycleManager/ILifecycleManager.h>

class LifecycleManager : public ILifecycleManager, public Component{

private:
	// <schedulerName, schedulerPtr>
	typedef Map<int, Scheduler*> MapSchedulers;
	typedef MapIterator<int, Scheduler*> IteratorMapScheduler;
	// <componentName, ComponentPtr>
	typedef Map<int, Component*> MapComponents;
	typedef MapIterator<int, Component*> IteratorMapComponents;
	// <componentName, schedulerName>
	typedef Map<int, int> MapAllocations;
	typedef MapIterator<int, int> IteratorMapAllocations;
	// <senderName, local name in sender>, ReceiverName
	typedef MapPair<int, unsigned> MapPairSender;
	typedef MapIterator<MapPairSender, int> IteratorMapSendersNReceivers;
	typedef Map<MapPairSender, int> MapSendersNReceivers;
	// <sourceName, groupName>, vector<tarGetName>*> SourcesNTargetsMap
	typedef MapPair<int, unsigned> MapPairSource;
	typedef Vector<int> VectorTargetNames;
	typedef MapIterator<MapPairSource, VectorTargetNames*> IteratorMapSourcesNTargets;
	typedef Map<MapPairSource, VectorTargetNames*> MapSourcesNTargets;

	MapSchedulers m_mapSchedulers;
	MapComponents m_mapComponents;
	MapAllocations m_mapAllocations;
	MapSendersNReceivers m_mapSendersNReceivers;
	MapSourcesNTargets	m_mapSourcesNTargets;

	// calback pointer to PMain
	Scheduler* m_pMainScheduler;

	void RegisterEventTypes();
	void RegisterExceptions();

	Scheduler* FindAScheduler(UId uId);
	Component* FindAComponent(UId uId);

public:
	// constructors & destructors
	LifecycleManager(int nClassId = _LifecycleManager_Id, const char* pcClassName = _LifecycleManager_Name);
	virtual ~LifecycleManager();

protected:
	void RegisterAScheduler(int name, Scheduler* pScheduler);
	virtual void RegisterSystemSchedulers();
	virtual void RegisterUserShedulers() = 0;
	void RegisterAComponent(int name, Component* pComponent);
	virtual void RegisterSystemComponents();
	virtual void RegisterUserComponents() = 0;
	void AllocateAComponent(int componentName, int schedulerName);
	virtual void AllocateSystemComponents();
	virtual void AllocateUserComponents() = 0;
	void AssociateASenderNAReceiver(int senderName, int receiverId, int receiverName);
	virtual void AssociateSystemSendersNReceivers();
	virtual void AssociateUserSendersNReceivers() = 0;
	void AssociateASourceNATarget(int nSourceName, int nGroupId, int nTarGetName);
	virtual void AssociateSystemSourcesNTargets();
	virtual void AssociateUserSourcesNTargets() = 0;

	virtual void InitializeComponents(Event* pEvent) = 0;
	virtual void StartSystem(Event* pEvent) = 0;
	virtual void StopSystem(Event* pEvent) = 0;

private: 
	//  InitializeAsALifecycleManager
	void InitializeAsALifecycleManager(Event* pEvent);	
	void RegisterSchedulers(Event* pEvent);	
	void InitializeSchedulers(Event* pEvent);	
	void StartSchedulers(Event* pEvent);	
	void RegisterComponents(Event* pEvent);
	void AllocateComponents(Event* pEvent);
	void AssociateSendersNReceivers(Event* pEvent);
	void AssociateSourcesNTargets(Event* pEvent);
	
	// FinalizeAsALifecycleManager
	void FinalizeAsALifecycleManager(Event* pEvent);
	void StopComponents(Event* pEvent);
	void FinalizeComponents(Event* pEvent);
	void StopSchedulers(Event* pEvent);
	void FinalizeSchedulers(Event* pEvent);
	void DeregisterComponents(Event* pEvent);	

protected:
	virtual void ProcessAEvent(Event* pEvent);
};
