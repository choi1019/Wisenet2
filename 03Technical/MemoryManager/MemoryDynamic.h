#pragma once

#include <03Technical/typedef.h>
#define _MemoryDynamic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryDynamic)
#define _MemoryDynamic_Name "MemoryDynamic"

#include <03Technical/MemoryManager/MemoryObject.h>
#include <03Technical/MemoryManager/PageList.h>


class MemoryDynamic :public MemoryObject
{
public:
	static void* 	s_pMemoryAllocated;
	static size_t 	s_szMemoryAllocated;
	static void* 	s_pMemoryCurrent;
	static size_t 	s_szMemoryCurrent;

	static PageList* s_pPageList;

	void* operator new(size_t szThis, void *pMemoryAllocated, size_t s_szMemoryAllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void *pMemoryAllocated, size_t s_szMemoryAllocated, const char* sMessage);

private:
	// attributes
	unsigned m_szPage;
	unsigned m_szUnit;
	unsigned m_szUnitExponentOf2;

	SlotList* m_pSlotListHead;
	
protected:
	virtual void* Malloc(size_t szObject, const char* sMessage);
	virtual bool Free(void* pObject);

public:
	// constructors and destructors
	MemoryDynamic(
		int nClassId = _MemoryDynamic_Id,
		const char* pClassName = _MemoryDynamic_Name);
	virtual ~MemoryDynamic();;

	virtual void Initialize(int szPage, int szSlotUnit);
	virtual void Finalize();

	SlotInfo *GetPSlotInfo(void *pObject);
	// maintenance
	void Show(const char* pTitle) override;
};
