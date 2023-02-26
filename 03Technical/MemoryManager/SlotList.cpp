#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Aspect/Log.h>

SlotList* SlotList::s_pSlotListRecycle = nullptr;
PageList* SlotList::s_pPageList = nullptr;

void* SlotList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotList = nullptr;
    if (SlotList::s_pSlotListRecycle == nullptr) {
        pNewSlotList = MemoryObject::s_pMemory->Malloc(szThis, sMessage);
    }
    else {
        pNewSlotList = SlotList::s_pSlotListRecycle;
        SlotList::s_pSlotListRecycle = SlotList::s_pSlotListRecycle->GetPNext();
        LOG_NEWLINE("SlotList::newRecycle(pNewSlotList)", (size_t)pNewSlotList);
    }
    return pNewSlotList;
}

void SlotList::operator delete(void* pObject) {
    SlotList* pSlotList = (SlotList*)pObject;
    pSlotList->SetPNext(SlotList::s_pSlotListRecycle);
    SlotList::s_pSlotListRecycle = pSlotList;
 //   LOG_NEWLINE("SlotList::delete(pObject)", (size_t)pObject);
}

void SlotList::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotList::delete", "_eNotSupport");
}

// for head
SlotList::SlotList(size_t szSlot, int nClassId,const char* pClassName)
    : MemoryObject(nClassId, pClassName)    
    , m_szSlot(szSlot)
    , m_numMaxSlots(0)
    , m_numPagesRequired(0)
    , m_pSlotListHead(this)

    , m_pPageIndex(nullptr)
    , m_idxPage(UNDEFINED)
    , m_numSlots(0)
    , m_pSlotHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotLists(0)

    , m_pSlotInfoHead(nullptr)

{
    // compute the number of pages required
    int szPage = s_pPageList->GetSzPage();
    // oversized slot bigger than a page
    m_numPagesRequired = m_szSlot / szPage;
    if (m_szSlot > m_numPagesRequired * szPage) {
        m_numPagesRequired++;
    }
    // compute the number of slots allocatable
    if (m_szSlot > 0) {
        this->m_numMaxSlots = m_numPagesRequired * szPage / m_szSlot;
    }
}
// for the rest
SlotList::SlotList(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szSlot(szSlot)
    , m_numMaxSlots(numMaxSlots)
     , m_numPagesRequired(numPagesRequired)
    , m_pSlotListHead(pSlotListHead)

    , m_pPageIndex(nullptr)
    , m_idxPage(0)
    , m_numSlots(0)
    , m_pSlotHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotLists(0)

    , m_pSlotInfoHead(nullptr)

{
    // allocate required number of pages
    this->m_pPageIndex = s_pPageList->AllocatePages(m_numPagesRequired, this);
    this->m_idxPage = this->m_pPageIndex->GetIndex();

    // set the number of slots allocatable
    this->m_numSlots = this->m_numMaxSlots;

    // generate slots
    this->m_pSlotHead = (Slot*)this->m_pPageIndex->GetPPage();
    Slot* pSlot = this->m_pSlotHead;
    Slot* pPreviousSlot = pSlot;
    for (int i = 0; i < m_numMaxSlots; i++) {
        pSlot->pNext = (Slot*)((size_t)pSlot + m_szSlot);
        pPreviousSlot = pSlot;
        pSlot = pSlot->pNext;
    }
    pPreviousSlot->pNext = nullptr;
}
SlotList::~SlotList() {
    if (this->m_idxPage != UNDEFINED) {
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
        pTargetSlotList = new("SlotList") SlotList(GetSzSlot(), GetNumMaxSlots(), GetNumPagesRequired(), this);
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
    MLOG_HEADER("SlotList::Show", "(szSlot, numMaxSlots, sMessage)", GetSzSlot(), GetNumMaxSlots(), sMessage);
    SlotList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
            // Slot* pSlot = pSibling->m_pSlotHead;
            // while (pSlot != nullptr) {
            //     // MLOG_NEWLINE("Slot-", (size_t)pSlot);
            //     pSlot = pSlot->pNext;
            // }
            MLOG_NEWLINE("Slot(", pSibling->GetIdxPage(), ")- numSlots = "
                        , pSibling->GetNumSlots(), pSibling->GetNumMaxSlots());
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("SlotList::Show");
};