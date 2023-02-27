#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <stdlib.h>

PageList* SlotList::s_pPageList = nullptr;
IMemory* SlotList::s_pMemory = nullptr;

SlotList** SlotList::s_apSlotList = nullptr;
SlotList* SlotList::s_pSlotListRecycle = nullptr;

void* SlotList::operator new(size_t szThis, void *pMemoryCurrent, const char* sMessage) {
    return pMemoryCurrent;
}
void* SlotList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotList = SlotList::s_pMemory->Malloc(szThis, sMessage);
    return pNewSlotList;
}

void SlotList::operator delete(void* pObject) {
    SlotList::s_pMemory->Free(pObject);
}

void SlotList::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotList::delete", "_eNotSupport");
}
void SlotList::operator delete(void* pObject, void *pMemoryCurrent, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotList::delete", "_eNotSupport");
}

// for head
SlotList::SlotList(size_t szSlot, SlotList *pSlotListHead, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szSlot(szSlot)
    , m_pSlotListHead(pSlotListHead)

    , m_pPageIndex(nullptr)
    , m_idxPage(UNDEFINED)
    , m_numPagesRequired(0)

    , m_numMaxSlots(0)
    , m_numSlots(0)
    , m_pSlotHead(nullptr)
    
    , m_bGarbage(false)
    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotLists(1)
    , m_pSlotInfoHead(nullptr)
{
    // not head node
    if (m_szSlot != 0) {
        int szPage = s_pPageList->GetSzPage();
        if (m_szSlot > szPage) {
            throw EXCEPTION(-1);
        }
        // compute the number of pages required
        m_numPagesRequired = m_szSlot / szPage;
        if (m_szSlot > m_numPagesRequired * szPage) {
            m_numPagesRequired++;
        }
        // compute the number of slots allocatable
        this->m_numMaxSlots = m_numPagesRequired * szPage / m_szSlot;    

        // allocate required number of pages
        this->m_pPageIndex = s_pPageList->AllocatePages(m_numPagesRequired, this);
        this->m_idxPage = this->m_pPageIndex->GetIndex();

        // set the number of slots allocatable
        this->m_numSlots = this->m_numMaxSlots;

        // allocate slots
        this->m_pSlotHead = (Slot*)this->m_pPageIndex->GetPPage();
        Slot* pPrevious = this->m_pSlotHead;
        Slot* pCurrent = pPrevious;
        for (int i = 0; i < m_numMaxSlots; i++) {
            pCurrent->pNext = (Slot*)((size_t)pCurrent + m_szSlot);
            pPrevious = pCurrent;
            pCurrent = pCurrent->pNext;
        }
        pPrevious->pNext = nullptr;
    }
}
SlotList::~SlotList() {
    if (m_szSlot != 0) {
        SlotList::s_pPageList->DelocatePages(this->m_idxPage);
    }
}
void SlotList::Initialize() {
    MemoryObject::Initialize();
}
void SlotList::Finalize() {
    MemoryObject::Finalize();
}

Slot *SlotList::AllocateASlot() {
    Slot* pSlot = this->m_pSlotHead;
    this->m_pSlotHead = this->m_pSlotHead->pNext;
    this->m_numSlots--;
    return pSlot;
}

void* SlotList::Malloc(size_t szObject, const char* sMessage) {
    SlotList *pTargetSlotList = nullptr;

    SlotList *pSibling = this;
    while (pSibling != nullptr) {
        if (pSibling->GetNumSlots() > 0) {
            pTargetSlotList = pSibling;
            break;
        }
        pSibling = pSibling->GetPSibling();
    }
    if (pTargetSlotList == nullptr) {
        // add a new SlotList
        pTargetSlotList = new("SlotList") SlotList(GetSzSlot(), this);
        SetCountSlotLists(GetCountSlotLists() + 1);
        // insert a new sibling
        pTargetSlotList->SetPSibling(this->GetPSibling());
        this->SetPSibling(pTargetSlotList);        
    }
    Slot *pTargetSlot = pTargetSlotList->AllocateASlot();
    NEW_DYNAMIC(sMessage, pTargetSlot,  "SlotList::Mallocc(szSlot, numSlots)"
                        , pTargetSlotList->GetSzSlot(), pTargetSlotList->GetNumSlots());
    return pTargetSlot; 
}

void SlotList::DelocateASlot(Slot* pSlotFree) {
    // insert pSlotFree to Slot LIst
    pSlotFree->pNext = m_pSlotHead;
    m_pSlotHead = pSlotFree;
    this->m_numSlots++;

    // check garbage
    if (m_numSlots == m_numMaxSlots) {
        // this is garbage
        this->m_bGarbage = true;
    } else {
        this->m_bGarbage = false;
    }
}
bool SlotList::Free(void* pObject) {
    size_t idxPage = s_pPageList->GetIdxPage(pObject);

    SlotList *pPrevious = this;
    SlotList *pCurrent = pPrevious->GetPSibling();
    while(pCurrent != nullptr) {
        if (pCurrent->GetIdxPage() == idxPage) {
            // found
            DELETE_DYNAMIC((size_t)pObject, pCurrent->GetIdxPage());
            pCurrent->DelocateASlot((Slot *)pObject);
            if (pCurrent->IsGarbage()) {
                pPrevious->SetPSibling(pCurrent->GetPSibling());
                SetCountSlotLists(GetCountSlotLists() - 1);
                MLOG_NEWLINE(pCurrent->GetClassName(), pCurrent->GetIdxPage());
                delete pCurrent;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    return false;
}

// maintenance
void SlotList::Show(const char* sMessage) { 
    // MLOG_HEADER("SlotList::Show", "(szSlot, numMaxSlots, sMessage)", GetSzSlot(), GetNumMaxSlots(), sMessage);
    // SlotList *pSibling = this->GetPSibling();
    // while (pSibling != nullptr) {
    //         // Slot* pSlot = pSibling->m_pSlotHead;
    //         // while (pSlot != nullptr) {
    //         //     // MLOG_NEWLINE("Slot-", (size_t)pSlot);
    //         //     pSlot = pSlot->pNext;
    //         // }
    //         MLOG_NEWLINE("SlotList(", pSibling->GetIdxPage(), ")- numSlots = "
    //                     , pSibling->GetNumSlots(), pSibling->GetNumMaxSlots());
    //     pSibling = pSibling->GetPSibling();
    // }
    // MLOG_FOOTER("SlotList::Show");
};