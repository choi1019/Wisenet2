#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <stdlib.h>

//-----------------------------------------------------------------------------------
PageList* SlotList::s_pPageList = nullptr;
IMemory* SlotList::s_pMemory = nullptr;

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

//-----------------------------------------------------------------------------------
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
    
    , m_nCountSlotLists(0)
    , m_bGarbage(false)
    , m_pNext(nullptr)
    , m_pSibling(nullptr)

    , m_pSlotInfoHead(nullptr)
{
    // not head node
    if (m_pSlotListHead != nullptr) {
        int szPage = s_pPageList->GetSzPage();
        // compute the number of pages required
        m_numPagesRequired = m_szSlot / szPage;
        if (m_szSlot > m_numPagesRequired * szPage) {
            m_numPagesRequired++;
        }
        // allocate required number of pages
        this->m_pPageIndex = s_pPageList->AllocatePages(m_numPagesRequired, this);
        this->m_idxPage = this->m_pPageIndex->GetIndex();

        // compute the number of slots allocatable
        this->m_numMaxSlots = m_numPagesRequired * szPage / m_szSlot;
        // set the number of slots allocatable
        this->m_numSlots = this->m_numMaxSlots;    

        // allocate slots
        this->m_pSlotHead = (Slot*)this->m_pPageIndex->GetPPage();
        Slot* pCurrent = m_pSlotHead;
        for (int i = 1; i < m_numMaxSlots; i++) {
            pCurrent->pNext = (Slot*)((size_t)pCurrent + m_szSlot);
            pCurrent = pCurrent->pNext;
        }
        pCurrent->pNext = nullptr;

    }
}

SlotList::~SlotList() {
    if (m_pSlotListHead != nullptr) {
        SlotList::s_pPageList->DelocatePages(this->m_idxPage);
    }
}

void SlotList::Initialize() {
    MemoryObject::Initialize();
}

void SlotList::Finalize() {
    MemoryObject::Finalize();
}

//-----------------------------------------------------------------------------------
SlotInfo* SlotList::FindSlotInfo(Slot* pSlot) {    
    SlotList *pSlotList = s_pPageList->GetPPageIndex(pSlot)->GetPSlotList();
    if (pSlotList == nullptr) {
        return nullptr;
    }
    SlotInfo *pPrevious = nullptr;
    SlotInfo *pCurrent = pSlotList->GetPSlotInfoHead();    
    while (pCurrent!=nullptr) {
        if (pCurrent->GetPSlot() == pSlot) {
            return pCurrent;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    return nullptr;
}

//-----------------------------------------------------------------------------------
void SlotList::AllocateASlotInfo(Slot *pSlot, const char* sMessage) {
    SlotInfo *pSlotInfo = new("SlotInfo") SlotInfo(pSlot, sMessage, this);
    if (m_pSlotListHead == nullptr) {
        return;
    }
    pSlotInfo->SetPNext(m_pSlotInfoHead);
    m_pSlotInfoHead = pSlotInfo;
}

Slot *SlotList::AllocateASlot(const char* sMessage) {
    Slot* pSlot = this->m_pSlotHead;
    this->m_pSlotHead = this->m_pSlotHead->pNext;
    this->m_numSlots--;
    AllocateASlotInfo(pSlot, sMessage);
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
    Slot *pTargetSlot = pTargetSlotList->AllocateASlot(sMessage);
    NEW_DYNAMIC(sMessage, pTargetSlot,  "SlotList::Mallocc(szSlot, numSlots)"
                        , pTargetSlotList->GetSzSlot(), pTargetSlotList->GetNumSlots());
    return pTargetSlot; 
}

//-----------------------------------------------------------------------------------
void SlotList::DelocateASlotInfo(Slot* pSlot) {    
    SlotList *pSlotList = s_pPageList->GetPPageIndex(pSlot)->GetPSlotList();
    if (pSlotList == nullptr) {
        return;
    }
    SlotInfo *pPrevious = nullptr;
    SlotInfo *pCurrent = pSlotList->GetPSlotInfoHead();    
    while (pCurrent!=nullptr) {
        if (pCurrent->GetPSlot() == pSlot) {
            if (pPrevious == nullptr) {
                pSlotList->SetPSlotInfoHead(pCurrent->GetPNext());
            } else {
                pPrevious->SetPNext(pCurrent->GetPNext());
            }
            delete pCurrent;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
}

void SlotList::DelocateASlot(Slot* pSlotFree) {
    // insert pSlotFree to Slot LIst
    pSlotFree->pNext = m_pSlotHead;
    m_pSlotHead = pSlotFree;
    this->m_numSlots++;

    DelocateASlotInfo(pSlotFree);

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

//-----------------------------------------------------------------------------------
// maintenance
void SlotList::Show(const char* sMessage) { 
    MLOG_HEADER("SlotList::Show", "(szSlot, numMaxSlots, sMessage)", GetSzSlot(), GetNumMaxSlots(), sMessage);
    SlotList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
            SlotInfo* pSlotInfo = pSibling->m_pSlotInfoHead;
            while (pSlotInfo != nullptr) {
                pSlotInfo->Show(sMessage);
                pSlotInfo = pSlotInfo->GetPNext();
            }
            MLOG_NEWLINE("SlotList(", pSibling->GetIdxPage(), ")- numSlots = "
                        , pSibling->GetNumSlots(), pSibling->GetNumMaxSlots());
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("SlotList::Show");
};