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

	// Synchronous
	bool m_bSynchronous;
//	bool m_bBlocked;
	// Sequential
//	bool m_bSequential;
	// Nested
//	bool m_bNested;

	// for working
	Event *m_pQueueNext;
//	Event *m_pSequenceNext;
	Event *m_pParent;
	unsigned m_countChildren;

public:
	Event(
		UId uidSource,
		UId uidTarget,
		int nType,
		long long lArg,
		ValueObject* pArg = nullptr,
		int nReplyType =UNDEFINED,
		
		bool bSynchronous = false,
//		bool bSequential = false,
//		bool bNested = false,

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

		, m_bReply(false)
		, m_nReplyType(nReplyType)

		, m_bSynchronous(bSynchronous)
//		, m_bBlocked(false)
//		, m_bSequential(bSequential)
//		, m_bNested(bNested)
		, m_pQueueNext(nullptr)
//		, m_pSequenceNext(nullptr)
		, m_pParent(nullptr)
		, m_countChildren(0)
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

		, m_bSynchronous(event.m_bSynchronous)
		// , m_bBlocked( event.m_bBlocked)
		// , m_bSequential(event.m_bSequential)
		// , m_bNested(event.m_bNested)
		, m_pQueueNext(event.m_pQueueNext)
//		, m_pSequenceNext(event.m_pSequenceNext)
		, m_pParent(event.m_pParent)
		, m_countChildren(event.m_countChildren)
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
	// bool IsBlocked() { return this->m_bBlocked; }
	// void SetBBlocked(bool bBlocked) { this->m_bBlocked = bBlocked; }
	// Sequence
	// bool IsSequential() { return this->m_bSequential; }
	// void SetBSequential(bool bSequential) { this->m_bSequential = bSequential; }
	// Nested
	// bool IsNested() { return this->m_bNested; }
	// void SetBNested(bool bNested) { this->m_bNested = bNested; }
	// int GetIdParent() { return this->m_idParent; }
	// void SetIdParent(int idParent) { this->m_idParent = idParent; }

	// Working - Serialization is not needed
	Event *GetPQueueNext() { return m_pQueueNext; }
	void SetPQueueNext(Event *pQueueNext) { m_pQueueNext = pQueueNext; }
//	Event *GetPSequenceNext() { return m_pSequenceNext; }
//	void SetPSequenceNext(Event *pSequenceNext) { m_pSequenceNext = pSequenceNext; }
	Event *GetPParent() { return m_pParent; }
	void SetPParent(Event *pParent) { m_pParent = pParent; }

	void IncrementCountChildren() { m_countChildren++; }
	void DecrementCountChildren() { m_countChildren--; }
	unsigned GetCoundChildren() { return m_countChildren; }
	void SetCountChildren(unsigned countChildren) { m_countChildren = countChildren; }
	bool IsAllReplied() {
		if (this->m_pParent!=nullptr) {
			if (this->m_pParent->GetCoundChildren() == 0) {
				return true;
			}
		}
		return false;
	}

	void Show(const char* sMessage, int nMessage = 0);
};