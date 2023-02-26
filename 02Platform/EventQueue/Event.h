#pragma once

#include <02Platform/typedef.h>
#define _Event_Id _GET_CLASS_UID(_ELayer_Platform::_eEvent)
#define _Event_Name "Event"

#include <01Base/Aspect/Directory.h>
#include <02Platform/Component/UId.h>
#include <01Base/Object/ValueObject.h>


class Event: public ValueObject
{
public:
	// static IMemory* s_pMemory;

	// void* operator new(size_t szThis, const char* sMessage);
	// void operator delete(void* pObject);
	// // dummy
	// void operator delete(void* pObject, const char* sMessage);

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
	// synchronous
	bool m_bSynchronous;
	// nested
	Event *m_pParent;
	unsigned m_countChildren;
	// event queue
	Event *m_pQueueNext;

public:
	Event(
		UId uidSource,
		UId uidTarget,
		int nType,
		long long lArg,
		ValueObject* pArg = nullptr,

		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
		) : ValueObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(uidSource)
		, m_uidTarget(uidTarget)
		, m_nType(nType)
		, m_lArg(lArg)
		, m_pPArg(pArg)

		, m_bReply(false)
		, m_nReplyType(UNDEFINED)

		, m_bSynchronous(false)
		, m_pParent(nullptr)
		, m_countChildren(0)
		, m_pQueueNext(nullptr)
	{
	}
	// copy constructor
	Event(
		const Event& event,

		unsigned uClassId = _Event_Id,
		const char* pcClassName = _Event_Name
		) : ValueObject(uClassId, pcClassName)
		, m_nId(s_uCounter++)
		, m_uidSource(event.m_uidSource)
		, m_uidTarget(event.m_uidTarget)
		, m_nType(event.m_nType)
		, m_lArg(event.m_lArg)
		, m_pPArg(m_pPArg == nullptr ? nullptr : event.m_pPArg->Clone())

		, m_bReply(event.m_bReply)
		, m_nReplyType(event.m_nReplyType)

		, m_bSynchronous(event.m_bSynchronous)
		, m_pParent(event.m_pParent)
		, m_countChildren(event.m_countChildren)
		, m_pQueueNext(event.m_pQueueNext)
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
	// Synchronous 
	bool IsSynchronous() { return this->m_bSynchronous; }
	void SetBSynchronous(bool bSynchronous) { this->m_bSynchronous = bSynchronous; }
	// nested
	Event *GetPParent() { return m_pParent; }
	void SetPParent(Event *pParent) { m_pParent = pParent; }
	void IncrementCountChildren() { m_countChildren++; }
	void DecrementCountChildren() { m_countChildren--; }
	unsigned GetCoundChildren() { return m_countChildren; }
	void SetCountChildren(unsigned countChildren) { m_countChildren = countChildren; }
	// event queue
	Event *GetPQueueNext() { return m_pQueueNext; }
	void SetPQueueNext(Event *pQueueNext) { m_pQueueNext = pQueueNext; }

	bool IsAllReplied() {
		if (this->m_pParent!=nullptr) {
			if (this->m_pParent->GetCoundChildren() == 0) {
				return true;
			}
		}
		return false;
	}

	// cloneable
	ValueObject* Clone() override {
		return new("Event::Clone") Event(*this);
	}

	// serializable
	char* Serialize() override {
		return nullptr;
	}
	void DeSerialize(char* pBuffer) override {
	}

	void Show(const char* sMessage, int nMessage = 0);
};