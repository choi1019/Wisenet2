#include <02Platform/EventQueue/Event.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

unsigned Event::s_uCounter = 0;

// IMemory* Event::s_pMemory = nullptr;

// void* Event::operator new (size_t szThis, const char* sMessage) {
//     void* pAllocated = Event::s_pMemory->Malloc(szThis, sMessage);
//     return pAllocated;
// }
// void Event::operator delete(void* pObject) {
//     Event::s_pMemory->Free(pObject);
// }
// // dummy
// void Event::operator delete(void* pObject, const char* sMessage) {
//      throw Exception((unsigned)IMemory::EException::_eNotSupport, "Event::delete", (size_t)pObject);
// }

void Event::Show(const char* sMessage, int nMessage) {
    LOG_NEWLINE(
        sMessage
        , nMessage
        , SHOW_COMPONENTNAME(this->GetUIdSource().GetComponentId())
        , SHOW_COMPONENTNAME(this->GetUIdTarget().GetComponentId())
        , SHOW_EVENTNAME(this->GetType())
        , SHOW_EVENTNAME(this->GetReplyType())
        , this->IsReply()
        );
}	