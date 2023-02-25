#pragma once

#include <03Technical/typedef.h>
#define _MemoryDynamic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryDynamic)
#define _MemoryDynamic_Name "MemoryDynamic"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/MemoryEven.h>

class MemoryDynamic :public MemoryObject
{
public:
	static PageList* s_pPageList;

	void* operator new(size_t szThis, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, const char* sMessage);

private:
	// attributes
	unsigned m_szUnit;
	MemoryEven* m_pMemoryEvenHead;
	unsigned m_szUnitExponentOf2;

protected:
	virtual void* Malloc(size_t szObject, const char* sMessage);
	virtual bool Free(void* pObject);

public:
	// constructors and destructors
	MemoryDynamic(
		unsigned szSlotUnit,
		int nClassId = _MemoryDynamic_Id,
		const char* pClassName = _MemoryDynamic_Name);
	virtual ~MemoryDynamic();;

	virtual void Initialize();
	virtual void Finalize();

	// maintenance
	void Show(const char* pTitle) override;
};
