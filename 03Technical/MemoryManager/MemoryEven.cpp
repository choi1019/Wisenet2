#include <03Technical/MemoryManager/MemoryEven.h>
#include <03Technical/MemoryManager/SlotInfo.h>
#include <01Base/Aspect/Log.h>

// for head MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int nClassId, const char* pClassName)
    : SlotList(szSlot, nClassId, pClassName)
    , m_pSlotInfoHead(nullptr)
    {
    }
// for normal MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
            int nClassId,     const char* pClassName)
    : SlotList(szSlot, numMaxSlots, numPagesRequired, pSlotListHead, nClassId, pClassName)
    , m_pSlotInfoHead(nullptr)
    {
    }
MemoryEven::~MemoryEven(){}

void MemoryEven::Initialize() {
    SlotList::Initialize();
}
void MemoryEven::Finalize() {
    SlotList::Finalize();
}

void MemoryEven::AddSlotInfo(Slot *pSlot, const char *sMessage) {
    SlotInfo *pSlotInfo = new("SlotInfo") SlotInfo(pSlot, sMessage, this);
    pSlotInfo->SetPNext(m_pSlotInfoHead);
    m_pSlotInfoHead = pSlotInfo;
    MLOG_NEWLINE("AddSlotInfo", (size_t)pSlot, sMessage);
}

void MemoryEven::DeleteSlotInfo(Slot *pSlot) {
    SlotInfo *pPrevious = nullptr;
    SlotInfo *pCurrent = m_pSlotInfoHead;
    while (pCurrent != nullptr) {
        if (pCurrent->GetPSlot() == pSlot) {
            if (pPrevious == nullptr) {
                m_pSlotInfoHead = pCurrent->GetPNext();
            } else {
                pPrevious->SetPNext(pCurrent->GetPNext());
            }
            MLOG_NEWLINE("DeleteSlotInfo", (size_t)pSlot, pCurrent->GetSMessage());
            delete pCurrent;
            return;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    LOG_NEWLINE("Error-MemoryEvent::Delete", (size_t)pSlot);
    throw EXCEPTION(-1);
}

SlotInfo *MemoryEven::GetPSlotInfo(Slot *pSlot) {
    SlotInfo *pSlotInfo = m_pSlotInfoHead;
    while (pSlotInfo != nullptr) {
        if (pSlotInfo->GetPSlot() == pSlot) {
            return pSlotInfo;
        }
    }
    return nullptr;
}

void* MemoryEven::Malloc(size_t szObject, const char* sMessage)
{
    MemoryEven *pTargetSlotList = nullptr;

    MemoryEven *pSibling = this;
    while (pSibling != nullptr) {
        if (pSibling->GetNumSlots() > 0) {
            pTargetSlotList = pSibling;
            break;
        }
        pSibling = (MemoryEven *)pSibling->GetPSibling();
    }
    if (pTargetSlotList == nullptr) {
        // add a new SlotList
        pTargetSlotList = new("MemoryEven") MemoryEven(GetSzSlot(), GetNumMaxSlots(), GetNumPagesRequired(), this);
        SetCountSlotLists(GetCountSlotLists() + 1);
        // insert a new sibling
        pTargetSlotList->SetPSibling(this->GetPSibling());
        this->SetPSibling(pTargetSlotList);        
    }
    Slot *pTargetSlot = pTargetSlotList->AllocateASlot();
    pTargetSlotList->AddSlotInfo((Slot *)pTargetSlot, sMessage);

    NEW_DYNAMIC(sMessage, pTargetSlot,  "(szSlot, numSlots)", this->GetSzSlot(), this->GetNumSlots());
    return pTargetSlot;
}

bool MemoryEven::Free(void* pObject) {
    size_t idxPage = s_pPageList->GetIdxPage(pObject);

    MemoryEven *pPrevious = this;
    MemoryEven *pCurrent = (MemoryEven *)pPrevious->GetPSibling();
    while(pCurrent != nullptr) {
        if (pCurrent->GetIdxPage() == idxPage) {
            // found
            pCurrent->DelocateASlot((Slot *)pObject);
            if (pCurrent->IsGarbage()) {
                pPrevious->SetPSibling(pCurrent->GetPSibling());
                SetCountSlotLists(GetCountSlotLists() - 1);
                delete pCurrent;
            }
            DELETE_DYNAMIC((size_t)pObject, this->GetIdxPage());
            pCurrent->DeleteSlotInfo((Slot *)pObject);
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = (MemoryEven *)pCurrent->GetPSibling();
    }
    return false;
}

// maintenance
void MemoryEven::Show(const char* sMessage) {
    SlotList::Show(sMessage);
    SlotList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
        MLOG_HEADER("MemoryEven::Show-Sibling", "SlotInfo(idxPage, numSlots)"
            , ((MemoryEven *)pSibling)->GetPSlotInfoHead()->GetPMemoryEven()->GetIdxPage()
            , ((MemoryEven *)pSibling)->GetPSlotInfoHead()->GetPMemoryEven()->GetNumSlots());
            SlotInfo* pSlotInfo = ((MemoryEven *)pSibling)->GetPSlotInfoHead();
            while (pSlotInfo!= nullptr) {
                pSlotInfo->Show(sMessage);
                pSlotInfo = pSlotInfo->GetPNext();
            }
        MLOG_FOOTER("SlotList::Show()-Sibling)");
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("MemoryEven-Show");
}
