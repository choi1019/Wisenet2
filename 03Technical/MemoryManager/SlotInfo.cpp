#include <03Technical/MemoryManager/SlotInfo.h>
#include <03Technical/MemoryManager/MemoryEven.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

IMemory* SlotInfo::s_pMemory = nullptr;

void* SlotInfo::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = SlotInfo::s_pMemory->Malloc(szThis, sMessage);
    return pAllocated;
}
void SlotInfo::operator delete(void* pObject) {
    SlotInfo::s_pMemory->Free(pObject);
}
// dummy
void SlotInfo::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotInfo::delete", (size_t)pObject);
}

SlotInfo::SlotInfo(Slot *pSlot, const char *sMessage, MemoryEven *pMemoryEven,
        int nObjectId, const char* sObjectName) 
    : RootObject(nObjectId, sObjectName)
    , m_pSlot(pSlot)
    , m_pMemoryEven(pMemoryEven)
    , m_pNext(nullptr)
{
    strcpy(m_sMessage, sMessage);
}
SlotInfo::~SlotInfo() {}
void SlotInfo::Initialize() {
    RootObject::Initialize();
}
void SlotInfo::Finalize() {
    RootObject::Finalize();
}

void SlotInfo::Show(const char* sMessage) {
    MLOG_NEWLINE(sMessage, ":", m_sMessage, "(pSlot=", (size_t)m_pSlot, ")-this=", (size_t)this);
}