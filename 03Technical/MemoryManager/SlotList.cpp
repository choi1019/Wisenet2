#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Aspect/Log.h>

SlotList* SlotList::s_pSlotListRecycle = nullptr;
PageList* SlotList::s_pPageList = nullptr;

void* SlotList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotList = nullptr;
    if (SlotList::s_pSlotListRecycle == nullptr) {
        pNewSlotList = MemoryObject::s_pMemory->SafeMalloc(szThis, sMessage);
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


SlotList::SlotList(size_t szSlot, int nClassId,const char* pClassName)
    : MemoryObject(nClassId, pClassName)    
    , m_szSlot(szSlot)
    , m_numMaxSlots(0)
    , m_numPagesRequired(0)
    , m_pSlotListHead(nullptr)

    , m_pPageIndex(nullptr)
    , m_idxPage(-1)
    , m_numSlots(0)
    , m_pSlotHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotLists(0)

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
    SlotList::s_pPageList->FreePages(this->m_idxPage);
}
void SlotList::Initialize() {
    MemoryObject::Initialize();
}
void SlotList::Finalize() {
    MemoryObject::Finalize();
}

Slot *SlotList::GetASlot() {
    Slot* pSlot = this->m_pSlotHead;
    this->m_pSlotHead = this->m_pSlotHead->pNext;
    this->m_numSlots--;
    return pSlot;
}

void* SlotList::Malloc(size_t szSlot, const char* sMessage) {
    SlotList *pTargetSlotList = nullptr;

    SlotList *pSibling = this;
    while (pSibling != nullptr) {
        if (pSibling->m_numSlots > 0) {
            pTargetSlotList = pSibling;
            break;
        }
        pSibling = pSibling->GetPSibling();
    }
    if (pTargetSlotList == nullptr) {
        // add a new SlotList
        pTargetSlotList = new("SlotList") SlotList(m_szSlot, m_numMaxSlots, m_numPagesRequired, this);
        m_nCountSlotLists++;
        // insert a new sibling
        pTargetSlotList->SetPSibling(this->GetPSibling());
        this->SetPSibling(pTargetSlotList);        
    }
    Slot *pTargetSlot = pTargetSlotList->GetASlot();
    NEW_DYNAMIC(sMessage, pTargetSlot,  "(szSlot, numSlots)", m_szSlot, m_numSlots);
    // SlotInfo *pSlotInfo = new("SlotInfo") SlotInfo(pTargetSlot, sMessage, this);
    return pTargetSlot; 
}

void SlotList::PutASlot(Slot* pSlotFree) {
    // insert pSlotFree to Slot LIst
    pSlotFree->pNext = m_pSlotHead;
    m_pSlotHead = pSlotFree;
    this->m_numSlots++;

    // check garbage
    if (m_numSlots == m_numMaxSlots) {
        // this is garbage
        this->m_bGarbage = true;
        m_nCountSlotLists--;			
    } else {
        this->m_bGarbage = false;
    }
}
bool SlotList::Free(void* pSlotFree) {
    size_t idxPage = s_pPageList->GetIdxPage(pSlotFree);

    SlotList *pPrevious = this;
    SlotList *pCurrent = pPrevious->GetPSibling();
    while(pCurrent != nullptr) {
        if (pCurrent->GetIdxPage() == idxPage) {
            // found
            pCurrent->PutASlot((Slot *)pSlotFree);
            if (pCurrent->IsGarbage()) {
                pPrevious->SetPSibling(pCurrent->GetPSibling());
                delete pCurrent;
            }
            DELETE_DYNAMIC((size_t)pSlotFree, idxPage);
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    return false;
}


void* SlotList::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}

bool SlotList::SafeFree(void* pObject) {
    Lock();
    bool result = this->Free(pObject);
    UnLock();
    return result;
}

// maintenance
void SlotList::Show(const char* pTitle) {
    MLOG_HEADER("SlotList-Head(", m_szSlot, ")::Show(numMaxSlots)", m_numMaxSlots, pTitle);
    SlotList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
        MLOG_HEADER("SlotList-Sibling-", pSibling->GetIdxPage(), "::Show(m_numSlots)", pSibling->GetNumSlots());
        Slot* pSlot = pSibling->m_pSlotHead;
        for (int i=0; i<pSibling->GetNumSlots(); i++) {
            MLOG_NEWLINE("Slot-", (size_t)pSlot);
            pSlot = pSlot->pNext;
        }
        MLOG_FOOTER("SlotList-Sibling)", pSibling->GetIdxPage(), "::Show()");
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("SlotList-Head(", m_szSlot, ")::Show()");
};