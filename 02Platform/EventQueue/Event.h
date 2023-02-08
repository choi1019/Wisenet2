#pragma once

#include <02Platform/typedef.h>
#define _Event_Id _GET_CLASS_UID(_ELayer_Platform::_eEvent)
#define _Event_Name "Event"

#include <01Base/Aspect/Directory.h>
#include <02Platform/Component/UId.h>
#include <01Base/Object/ValueObject.h>

class Event: public ValueObject
{
private:
	static unsigned s_uCounter;

	int m_nId;
	UId m_uidSource;
	UId m_uidTarget;
	int m_nType;
	// Arguments
	long long m_lArg;
	ValueObject* m_pPArg;

	// for Reply
	bool m_bReply;
	int m_nReplyType;
	// Sequenced or not
	bool m_bSequenced;
	Event* m_pNext;
	// Nested
	bool m_bNested;
	Event* m_pParent;
	unsigned m_countChildren;
	// Blocked or not
	bool m_bBlocked;
	// For Event Queue
	Event* m_pNextInQueue;

public:
	Event(
		UId uidSource,
		UId uidTarget,
		int nType,
		long long lArg,
		ValueObject* pArg = nullptr,
		
		bool bReply = false,	
		bool bSequenced = true,
		bool bNested = false,

		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
	)
		: ValueObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(uidSource)
		, m_uidTarget(uidTarget)
		, m_nType(nType)
		, m_lArg(lArg)
		, m_pPArg(pArg)

		, m_bReply(bReply)
		, m_nReplyType(UNDEFINED)
		, m_bSequenced(bSequenced)
		, m_pNext(nullptr)
		, m_bNested(bSequenced)
		, m_pParent(nullptr)
		, m_countChildren(0)
		, m_bBlocked(false)
		, m_pNextInQueue(nullptr)
	{
	}

	Event(
		const Event& event,
		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
	)
		: ValueObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(event.m_uidSource)
		, m_uidTarget(event.m_uidTarget)
		, m_nType(event.m_nType)
		, m_lArg(event.m_lArg)
		, m_pPArg(m_pPArg == nullptr ? nullptr : event.m_pPArg->Clone())

		, m_bReply(event.m_bReply)
		, m_nReplyType(event.m_nReplyType)
		, m_bSequenced(event.m_bSequenced)
		, m_pNext(event.m_pNext)
		, m_bNested(event.m_bNested)
		, m_pParent(event.m_pParent)
		, m_countChildren(event.m_countChildren)
		, m_bBlocked( event.m_bBlocked)
		, m_pNextInQueue( event.m_pNextInQueue) 
	{
	}
	virtual ~Event() {
		if (m_pPArg != nullptr) {
			delete m_pPArg;
		}
	}

	void Initialize() override {
		ValueObject::Initialize();
	}
	void Finalize() override {
		ValueObject::Finalize();
	}

	int GetId() { return this->m_nId; }
	UId GetUIdSource() { return this->m_uidSource; }
	UId GetUIdTarget() { return this->m_uidTarget; }
	int GetType() { return this->m_nType; }
	long long GetlArg() { return this->m_lArg; }
	ValueObject* GetPArg() { return this->m_pPArg; }

	void SetUIdSource(UId uidSource) { this->m_uidSource = uidSource; }
	void SetUIdTarget(UId uidTarget) { this->m_uidTarget = uidTarget; }
	void SetType(int nType) { this->m_nType = nType; }
	void SetlArg(long long lArg) { this->m_lArg = lArg; }
	void SetPArg(ValueObject* pPArg) { this->m_pPArg = pPArg; }

	// Reply
	bool IsReply() { return this->m_bReply; }
	void SetBReply(bool bReply) { this->m_bReply = bReply; }
	int GetReplyType() { return this->m_nReplyType; }
	void SetReplyType(int nReplyType) { this->m_nReplyType = nReplyType; }
	// Sequence
	bool IsSequenced() { return this->m_bSequenced; }
	void SetBSequenced(bool bSequenced) { this->m_bSequenced = bSequenced; }
	Event* GetPNext() { return this->m_pNext; }
	void SetPNext(Event* pNext) { this->m_pNext = pNext; }
	// Nested
	bool IsNested() { return this->m_bNested; }
	void SetBNested(bool bNested) { this->m_bNested = bNested; }
	Event* GetPParent() { return this->m_pParent; }
	void SetPParent(Event* pParent) { this->m_pParent = pParent; }
	void IncrementCountChildren() { this->m_countChildren++; }
	void DecrementCountChildren() { this->m_countChildren--; }
	unsigned GetCoundChildren() { return this->m_countChildren; }
	void SetCountChildren(unsigned countChildren) { this->m_countChildren = countChildren; }
	bool IsAllReplied() {
		if (this->m_bNested) {
			if (this->m_pParent->GetCoundChildren() > 0) {
				return false;
			}
		}
		return true;
	}
	// For Event Queue
	Event* GetPNextInQueue() { return this->m_pNextInQueue; }
	void SetPNextInQueue(Event* pNextInQueue) { this->m_pNextInQueue = pNextInQueue; }
};