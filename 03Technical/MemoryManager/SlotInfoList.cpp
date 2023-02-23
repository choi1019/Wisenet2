#include <03Technical/MemoryManager/SlotInfoList.h>
#include <01Base/Aspect/Log.h>

SlotInfoList* SlotInfoList::s_pSlotInfoListRecycle = nullptr;
PageList* SlotInfoList::s_pPageList = nullptr;

void* SlotInfoList::operator new(size_t szThis, const char* sMessage) {
    void* pNewSlotInfoList = nullptr;
    if (SlotInfoList::s_pSlotInfoListRecycle == nullptr) {
        pNewSlotInfoList = MemoryObject::s_pMemory->SafeMalloc(szThis, sMessage);
    }
    else {
        pNewSlotInfoList = SlotInfoList::s_pSlotInfoListRecycle;
        SlotInfoList::s_pSlotInfoListRecycle = SlotInfoList::s_pSlotInfoListRecycle->GetPNext();
        LOG_NEWLINE("SlotInfoList::newRecycle(pNewSlotInfoList)", (size_t)pNewSlotInfoList);
    }
    return pNewSlotInfoList;
}

void SlotInfoList::operator delete(void* pObject) {
    SlotInfoList* pSlotInfoList = (SlotInfoList*)pObject;
    pSlotInfoList->SetPNext(SlotInfoList::s_pSlotInfoListRecycle);
    SlotInfoList::s_pSlotInfoListRecycle = pSlotInfoList;
 //   LOG_NEWLINE("SlotInfoList::delete(pObject)", (size_t)pObject);
}

void SlotInfoList::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotInfoList::delete", "_eNotSupport");
}


SlotInfoList::SlotInfoList(size_t szSlotInfo, int nClassId,const char* pClassName)
    : MemoryObject(nClassId, pClassName)    
    , m_szSlotInfo(szSlotInfo)
    , m_numMaxSlotInfos(0)
    , m_szPage(0)
    , m_numPagesRequired(0)
    , m_pSlotInfoListHead(nullptr)

    , m_pPageIndex(nullptr)
    , m_idxPage(-1)
    , m_numSlotInfos(0)
    , m_pSlotInfoHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotInfoLists(0)

{
    // compute the number of pages required
    m_szPage = s_pPageList->GetSzPage();
    // oversized SlotInfo bigger than a page
    m_numPagesRequired = m_szSlotInfo / m_szPage;
    if (m_szSlotInfo > m_numPagesRequired * m_szPage) {
        m_numPagesRequired++;
    }
    // compute the number of SlotInfos allocatable
    if (m_szSlotInfo > 0) {
        this->m_numMaxSlotInfos = m_numPagesRequired * m_szPage / m_szSlotInfo;
    }
}

SlotInfoList::SlotInfoList(size_t szSlotInfo, int numMaxSlotInfos, int szPage, int numPagesRequired, SlotInfoList *pSlotInfoListHead, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szSlotInfo(szSlotInfo)
    , m_numMaxSlotInfos(numMaxSlotInfos)
    , m_szPage(szPage)
    , m_numPagesRequired(numPagesRequired)
    , m_pSlotInfoListHead(pSlotInfoListHead)

    , m_pPageIndex(nullptr)
    , m_idxPage(0)
    , m_numSlotInfos(0)
    , m_pSlotInfoHead(nullptr)
    , m_bGarbage(false)

    , m_pNext(nullptr)
    , m_pSibling(nullptr)
    , m_nCountSlotInfoLists(0)

{

    // allocate required number of pages
    this->m_pPageIndex = s_pPageList->AllocatePages(m_numPagesRequired);
    this->m_idxPage = this->m_pPageIndex->GetIndex();

    // set the number of SlotInfos allocatable
    this->m_numSlotInfos = this->m_numMaxSlotInfos;

    // generate SlotInfos
    this->m_pSlotInfoHead = (SlotInfo*)this->m_pPageIndex->GetPPage();
    SlotInfo* pSlotInfo = this->m_pSlotInfoHead;
    SlotInfo* pPreviousSlotInfo = pSlotInfo;
    for (int i = 0; i < m_numMaxSlotInfos; i++) {
        pSlotInfo->SetPNext((SlotInfo*)((size_t)pSlotInfo + m_szSlotInfo));
        pPreviousSlotInfo = pSlotInfo;
        pSlotInfo = pSlotInfo->GetPNext();
    }
    pPreviousSlotInfo->SetPNext(nullptr);
}
SlotInfoList::~SlotInfoList() {
    SlotInfoList::s_pPageList->FreePages(this->m_idxPage);
}
void SlotInfoList::Initialize() {
    MemoryObject::Initialize();
}
void SlotInfoList::Finalize() {
    MemoryObject::Finalize();
}

SlotInfo *SlotInfoList::GetASlotInfo() {
    SlotInfo* pSlotInfo = this->m_pSlotInfoHead;
    this->m_pSlotInfoHead = this->m_pSlotInfoHead->GetPNext();
    this->m_numSlotInfos--;
    return pSlotInfo;
}

void* SlotInfoList::Malloc(size_t szSlotInfo, const char* sMessage) {
    SlotInfoList *pTargetSlotInfoList = nullptr;

    SlotInfoList *pSibling = this;
    while (pSibling != nullptr) {
        if (pSibling->m_numSlotInfos > 0) {
            pTargetSlotInfoList = pSibling;
            break;
        }
        pSibling = pSibling->GetPSibling();
    }
    if (pTargetSlotInfoList == nullptr) {
        // add a new SlotInfoList
        pTargetSlotInfoList = new("SlotInfoList") SlotInfoList(m_szSlotInfo, m_numMaxSlotInfos, m_szPage, m_numPagesRequired, this);
        m_nCountSlotInfoLists++;
        // insert a new sibling
        pTargetSlotInfoList->SetPSibling(this->GetPSibling());
        this->SetPSibling(pTargetSlotInfoList);        
    }
    SlotInfo *pTargetSlotInfo = pTargetSlotInfoList->GetASlotInfo();
    NEW_DYNAMIC(sMessage, pTargetSlotInfo,  "(szSlotInfo, numSlotInfos)", m_szSlotInfo, m_numSlotInfos);
//    SlotInfo *pSlotInfo = new("SlotInfoInfo") SlotInfo(pTargetSlotInfo, sMessage, this);
    return pTargetSlotInfo; 
}

void SlotInfoList::PutASlotInfo(SlotInfo* pSlotInfoFree) {
    // insert pSlotInfoFree to SlotInfo LIst
    pSlotInfoFree->SetPNext(m_pSlotInfoHead);
    m_pSlotInfoHead = pSlotInfoFree;
    this->m_numSlotInfos++;

    // check garbage
    if (m_numSlotInfos == m_numMaxSlotInfos) {
        // this is garbage
        this->m_bGarbage = true;
        m_nCountSlotInfoLists--;			
    } else {
        this->m_bGarbage = false;
    }
}
bool SlotInfoList::Free(void* pSlotInfoFree) {
    size_t idxPage = s_pPageList->GetIdxPage(pSlotInfoFree);

    SlotInfoList *pPrevious = this;
    SlotInfoList *pCurrent = pPrevious->GetPSibling();
    while(pCurrent != nullptr) {
        if (pCurrent->GetIdxPage() == idxPage) {
            // found
            pCurrent->PutASlotInfo((SlotInfo *)pSlotInfoFree);
            if (pCurrent->IsGarbage()) {
                pPrevious->SetPSibling(pCurrent->GetPSibling());
                delete pCurrent;
            }
            DELETE_DYNAMIC((size_t)pSlotInfoFree, idxPage);
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    return false;
}


void* SlotInfoList::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}

bool SlotInfoList::SafeFree(void* pObject) {
    Lock();
    bool result = this->Free(pObject);
    UnLock();
    return result;
}

// maintenance
void SlotInfoList::Show(const char* pTitle) {
    MLOG_HEADER("SlotInfoList-Head(", m_szSlotInfo, ")::Show(numMaxSlotInfos)", m_numMaxSlotInfos, pTitle);
    SlotInfoList *pSibling = this->GetPSibling();
    while (pSibling != nullptr) {
        MLOG_HEADER("SlotInfoList-Sibling-", pSibling->GetIdxPage(), "::Show(m_numSlotInfos)", pSibling->GetNumSlotInfos());
        SlotInfo* pSlotInfo = pSibling->m_pSlotInfoHead;
        for (int i=0; i<pSibling->GetNumSlotInfos(); i++) {
            MLOG_NEWLINE("SlotInfo-", (size_t)pSlotInfo);
            pSlotInfo = pSlotInfo->GetPNext();
        }
        MLOG_FOOTER("SlotInfoList-Sibling)", pSibling->GetIdxPage(), "::Show()");
        pSibling = pSibling->GetPSibling();
    }
    MLOG_FOOTER("SlotInfoList-Head(", m_szSlotInfo, ")::Show()");
};