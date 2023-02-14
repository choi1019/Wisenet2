#include <02Platform/EventQueue/Event.h>
#include <01Base/Aspect/Log.h>

unsigned Event::s_uCounter = 0;

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