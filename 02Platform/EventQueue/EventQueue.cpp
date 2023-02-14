#include <02Platform/EventQueue/EventQueue.h>
#include <01Base/Aspect/Log.h>

void EventQueue::Show(const char* sMessage) {
    LOG_HEADER0();
    //		Log("SizeThis + Size Allocated", this->GetSzThis(), m_szSlot * m_uCountTotalSlots).Println();
    //		Log("SizeSlot x CountSlots", m_szSlot, m_uCountTotalSlots).Println();
    unsigned index = 0;
    for (Event* pEvent = m_pFront; pEvent != nullptr; pEvent = pEvent->GetPQueueNext()) {
        pEvent->Show(sMessage);
    }
    LOG_FOOTER0();
}