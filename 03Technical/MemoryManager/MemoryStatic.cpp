#include <03Technical/MemoryManager/MemoryStatic.h>

#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Object/BaseObject.h>

void* MemoryStatic::s_pAllocated = nullptr;
size_t MemoryStatic::s_szAllocated = 0;
void* MemoryStatic::s_pCurrent = nullptr;
size_t MemoryStatic::s_szCurrent = 0;

void* MemoryStatic::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryllocated, const char* sMessage) {
    if (szMemoryllocated < szThis) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreStaticMemory, "new MemoryStatic", "_eNoMoreSystemMemory");
    }
    s_pAllocated = pMemoryAllocated;
    s_szAllocated = szMemoryllocated;
    s_pCurrent = (void*)((size_t)s_pAllocated + szThis);
    s_szCurrent = s_szAllocated - szThis;

    // MLOG_NEWLINE("MemoryStatic::new", sMessage, szThis, s_szCurrent, s_szAllocated);
    return s_pAllocated;
}
void MemoryStatic::operator delete(void* pObject) {
}
void MemoryStatic::operator delete(void* pObject, void* pMemoryAllocated, size_t szMemoryllocated) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryStatic::delete", "_eNotSupport");
}

MemoryStatic::MemoryStatic(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
{
    // set memory manager of RootObject as this
    BaseObject::s_pMemory = this;
}
MemoryStatic::~MemoryStatic() {
//    SHOW_STATIC("MemoryStatic::~MemoryStatic");
 }
void MemoryStatic::Initialize() {
    MemoryObject::Initialize();
}
void MemoryStatic::Finalize() {
    MemoryObject::Finalize();
}

// methods
void* MemoryStatic::Malloc(size_t szObject, const char* sMessage) {
    if (s_szCurrent < szObject) {
        throw Exception((unsigned)IMemory::EException::_eNoMoreStaticMemory, sMessage, "MemoryStatic::Malloc", "_eNoMoreSystemMemory");
    }    
    void* pMemoryAllocated = s_pCurrent;
    s_pCurrent = (void*)((size_t)s_pCurrent + szObject);
    s_szCurrent = s_szCurrent - szObject;

    MLOG_NEWLINE(sMessage, (size_t)pMemoryAllocated, "(szObject, s_szCurrent, s_szAllocated)", szObject, s_szCurrent, s_szAllocated);
    return pMemoryAllocated;
}
bool MemoryStatic::Free(void* pObject) {
    return true;
}

// maintenance
void MemoryStatic::Show(const char* pTitle) {
    // MLOG_HEADER("MemoryStatic::Show", pTitle);
    // MLOG_NEWLINE("MemoryStatic(szAllocated,pAllocated,szCurrent,pCurrent)"
    // 	, s_szAllocated
    // 	, (size_t)s_pAllocated
    //     , s_szCurrent
    // 	, (size_t)s_pCurrent
    // );
    // MLOG_FOOTER("MemoryStatic::Show", pTitle);
}