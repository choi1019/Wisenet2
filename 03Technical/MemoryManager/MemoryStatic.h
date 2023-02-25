#pragma once

#include <03Technical/typedef.h>
#define _MemoryStatic_Id _GET_CLASS_UID(_ELayer_Technical::_eMemoryStatic)
#define _MemoryStatic_Name "MemoryStatic"

#include <03Technical/MemoryManager/MemoryObject.h>

class MemoryStatic : public MemoryObject {
public:
	// memory for static allocation - no delete
	static void* s_pAllocated;
	static size_t s_szAllocated;
	static void* s_pCurrent;
	static size_t s_szCurrent;

	void* operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated, const char* sMessage);
	void operator delete(void* pObject);
	void operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated);

public:
	MemoryStatic(
		int nClassId = _MemoryStatic_Id,
		const char* pClassName = _MemoryStatic_Name);
	virtual ~MemoryStatic();
	virtual void Initialize();
	virtual void Finalize();

	// methods
	virtual void* Malloc(size_t szAllocate, const char* sMessage);
	virtual bool Free(void* pObject);

	// maintenance
	void Show(const char* sMessage) override;
};