#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotList.h>
	
void* PageList::s_pMemeoryAllocated = nullptr;
size_t PageList::s_szMemoryAllocated = 0;
void* PageList::s_pMemeoryCurrent = nullptr;
size_t PageList::s_szMemoryCurrent = 0;

void* PageList::operator new(size_t szThis, void *pApplicationMemeory, size_t szApplicationMemory, const char* sMessage) {
    // allocate this
    s_pMemeoryAllocated = pApplicationMemeory;
    s_szMemoryAllocated = szApplicationMemory;
    s_pMemeoryCurrent = (void*)((size_t)s_pMemeoryAllocated + szThis);
    s_szMemoryCurrent = s_szMemoryAllocated - szThis;

    return s_pMemeoryAllocated;
}
void PageList::operator delete(void* pObject) {
    // delete this
 }
void PageList::operator delete(void* pObject, void *pApplicationMemeory, size_t szApplicationMemory, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "PageList::delete", __LINE__);
}

PageList::PageList(
    size_t szPage,
    int nClassId,
    const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szPage(szPage)
    , m_pStartPage(nullptr)
{
    if (s_szMemoryCurrent < (sizeof(PageIndex*) + sizeof(PageIndex) + m_szPage)) {
        throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage)
                            , "PageList", "PageList", "MemoryTooSmall");
    }
    // compute number of pages, szTotalMemory = (szP-PageIndex + szPageIndex + szPage) * numPages
    m_numPagesAllocated = s_szMemoryCurrent / (sizeof(PageIndex*) + sizeof(PageIndex) + m_szPage);
    m_numPagesCurrent = m_numPagesAllocated;

    // allocate PageIndex Pointer Array
    m_apPageIndices = (PageIndex**)s_pMemeoryCurrent;
    s_pMemeoryCurrent = (void*)((size_t)s_pMemeoryCurrent + (sizeof(PageIndex*) * m_numPagesAllocated));

    // compute start address of real pages
    m_pStartPage =  (void*)((size_t)s_pMemeoryCurrent + sizeof(PageIndex) * m_numPagesAllocated);
    // alllocate PageIndex Array and allocate a real page
    void *pPage = m_pStartPage;
    for (unsigned index = 0; index < m_numPagesAllocated; index++) {
        m_apPageIndices[index] = new(s_pMemeoryCurrent, "PageIndex") PageIndex(GetIdxPage(pPage), pPage);
        s_pMemeoryCurrent = (void *)((size_t)s_pMemeoryCurrent + sizeof(PageIndex));
        pPage = (Page*)((size_t)pPage + m_szPage);
    }
    // compute the size allocated
    size_t szTotalAllocated = m_numPagesAllocated * (sizeof(PageIndex*) + sizeof(PageIndex) + m_szPage);
    s_szMemoryCurrent = s_szMemoryCurrent - szTotalAllocated;
}

PageList::~PageList() {
}
void PageList::Initialize() {
    MemoryObject::Initialize();
    SlotList::s_pPageList = this;
    
}
void PageList::Finalize() {
    MemoryObject::Finalize();
}

PageIndex* PageList::AllocatePages(unsigned numPagesRequired, SlotList *pSlotList) {
    if (m_numPagesCurrent < numPagesRequired) {
        throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
    } else {
        // if numPagesRequired > 1
        unsigned numPagesAllocated = numPagesRequired;
        unsigned indexFound = 0;
        for (unsigned index = 0; index < m_numPagesAllocated; index++) {
            if ((m_apPageIndices[index]->IsAllocated())) {
                // if the page is discontinued, reset index
                numPagesAllocated = numPagesRequired;
                indexFound = index + 1;
            }
            else {
                numPagesAllocated--;
                if (numPagesAllocated == 0) {
                    // found - page info is set in a head page
                    m_apPageIndices[indexFound]->SetNumAllocated(numPagesRequired);
                    m_apPageIndices[indexFound]->SetPSlotList(pSlotList);
                    for (unsigned i = 0; i < numPagesRequired; i++) {
                        // multiple pages needed
                        m_apPageIndices[indexFound + i]->SetIsAllocated(true);
                    }
                    m_numPagesCurrent = m_numPagesCurrent - numPagesRequired;
                    return m_apPageIndices[indexFound];
                }
            }
        }
        // not found
        // need compaction =====
        throw Exception((unsigned)IMemory::EException::_eNoMorePage, "Memory", "Malloc", "_eNoMorePage");
    }
}

void PageList::DelocatePages(unsigned indexFree) {
    unsigned numPagesAllocated = m_apPageIndices[indexFree]->GetNumAllocated();
    for (unsigned i = 0; i < numPagesAllocated; i++) {
        m_apPageIndices[indexFree + i]->SetNumAllocated(1);
        m_apPageIndices[indexFree + i]->SetIsAllocated(false);
    }
    m_numPagesCurrent = m_numPagesCurrent + numPagesAllocated;
}

void PageList::Show(const char* pTitle) {
    MLOG_HEADER("PageList::Show(szPage,numPagesAvaiable,numPagesMax)", m_szPage, m_numPagesCurrent, m_numPagesAllocated);
    for (unsigned i=0; i< m_numPagesAllocated; i++) {
        m_apPageIndices[i]->Show("Page");
    }
    MLOG_FOOTER("PageList::Show");
}