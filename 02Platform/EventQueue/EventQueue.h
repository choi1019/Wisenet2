#pragma once

#include <02Platform/typedef.h>
#define _EventQueue_Id _GET_CLASS_UID(_ELayer_Platform::_eEventQueue)
#define _EventQueue_Name "EventQueue"

#include <01Base/Object/BaseObject.h>
#include <01Base/Object/ValueObject.h>
#include <02Platform/EventQueue/Event.h>

class EventQueue : public BaseObject
{
private:
	int m_nSchedulerId;
	Event* m_pFront;
	Event* m_pRear;
	unsigned m_nLength;
protected:
	virtual void PushLock() = 0;
	virtual void PushUnlock() = 0;
	virtual void PopLock() = 0;
	virtual void PopUnlock() = 0;

public:
	int GetSchedulerId() { return this->m_nSchedulerId; }

	EventQueue(
		int uSchedulerId = UNDEFINED,
		int uClassId = _EventQueue_Id,
		const char* pcClassName = _EventQueue_Name)
		: BaseObject(uClassId, pcClassName)
		, m_nSchedulerId(uSchedulerId)
		, m_pFront(nullptr)
		, m_pRear(nullptr)
		, m_nLength(0)
	{
	}
	virtual ~EventQueue() 
	{
	}
	virtual void Initialize() {
		BaseObject::Initialize();
	}
	virtual void Finalize() {
		BaseObject::Finalize();
	}

	virtual void PushBack(Event* pEvent) {
		this->PushLock();
		if (m_nLength == 0) {
			this->m_pRear = pEvent;
			this->m_pFront = pEvent;
		}
		else {
			this->m_pRear->SetPQueueNext(pEvent);
			this->m_pRear = pEvent;
		}
		this->m_nLength++;
		this->PushUnlock();
	}
	
	virtual Event* Front() {
		return this->m_pFront;
	}

	virtual Event* PopFront(){
		this->PopLock();
		Event *pEvent = m_pFront;
		this->m_pFront = m_pFront->GetPQueueNext();
		this->m_nLength--;
		this->PopUnlock();
		return pEvent;	
	}

	virtual bool IsEmpty() {
		return (m_nLength == 0) ? true : false;
	}

	void Show(const char* sMessage);
};

