#include <03Technical/MemoryManager/SlotInfo.h>

IMemory* SlotInfo::s_pMemory = nullptr;

void* SlotInfo::operator new (size_t szThis, const char* sMessage) {
    void* pAllocated = SlotInfo::s_pMemory->SafeMalloc(szThis, sMessage);
    return pAllocated;
}
void SlotInfo::operator delete(void* pObject) {
    SlotInfo::s_pMemory->SafeFree(pObject);
}
// dummy
void SlotInfo::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotInfo::delete", (size_t)pObject);
}

void SlotInfo::Show(const char* sMessage) {
    MLOG_NEWLINE(sMessage);
}	