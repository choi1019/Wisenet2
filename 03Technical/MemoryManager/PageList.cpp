#include <03Technical/MemoryManager/PageList.h>
#include <03Technical/MemoryManager/SlotList.h>
	
void* PageList::operator new(size_t szThis, void *pMemoryAllocated, const char* sMessage) {
    return pMemoryAllocated;
}
void PageList::operator delete(void* pObject) {
    // delete this
 }
void PageList::operator delete(void* pObject,  void *pMemoryAllocated, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "PageList::delete", __LINE__);
}

PageList::PageList(    
    int nClassId,
    const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_pMemoryCurrent(nullptr)
    , m_szMemoryCurrent(0)
    , m_numPagesAllocated(0)
    , m_numPagesCurrent(0)
    , m_apPageIndices(nullptr)
    , m_szPage(0)
    , m_pPageHead(nullptr)
{
}

PageList::~PageList() {
}

size_t PageList::Initialize(void *pMemoryAllocated, size_t szMemoryAllocated, size_t szPage) {
    MemoryObject::Initialize();
    m_pMemoryAllocated = pMemoryAllocated;
    m_szMemoryAllocated = szMemoryAllocated;
    m_szPage = szPage;

    m_pMemoryCurrent = m_pMemoryAllocated;
    m_szMemoryCurrent = m_szMemoryAllocated;
    if (m_szMemoryCurrent < (sizeof(PageIndex*) + sizeof(PageIndex) + m_szPage)) {
        throw Exception((unsigned)(IMemory::EException::_eMemoryAllocatedIsSmallerThanAPage)
                            , "PageList", "PageList", "MemoryTooSmall");
    }

    // compute szTotalMemory = (szP-PageIndex + szPageIndex + szPage) * numPages
    m_numPagesAllocated = m_szMemoryCurrent / (sizeof(PageIndex*) + sizeof(PageIndex) + m_szPage);
    m_numPagesCurrent = m_numPagesAllocated;

    // allocate PageIndex Pointer Array
    m_apPageIndices = (PageIndex**)m_pMemoryCurrent;
        m_pMemoryCurrent = (void*)((size_t)m_pMemoryCurrent + (sizeof(PageIndex*) * m_numPagesAllocated));
        m_szMemoryCurrent = m_szMemoryCurrent - (sizeof(PageIndex*) * m_numPagesAllocated);

    // allocate PageIndices
    for (unsigned index = 0; index < m_numPagesAllocated; index++) {
        m_apPageIndices[index] = new(m_pMemoryCurrent, "PageIndex") PageIndex();
        m_pMemoryCurrent = (void *)((size_t)m_pMemoryCurrent + sizeof(PageIndex));
    }
    m_szMemoryCurrent = m_szMemoryCurrent - (sizeof(PageIndex) * m_numPagesAllocated);

    // alllocate Pages 
    m_pPageHead = m_pMemoryCurrent;
    Page *pPage = (Page*) m_pPageHead;
    for (unsigned index = 0; index < m_numPagesAllocated; index++) {
        m_apPageIndices[index]->Initialize(GetIdxPage(pPage), pPage);
        pPage = (Page*)((size_t)pPage + m_szPage);
    }
    m_szMemoryCurrent = m_szMemoryCurrent - (m_szPage * m_numPagesAllocated);

    // compute total size allocated
    size_t szTotalAllocated = m_szMemoryAllocated - m_szMemoryCurrent;    
    return szTotalAllocated;
}
void PageList::Finalize() {
    MemoryObject::Finalize();
}

PageIndex* PageList::AllocatePages(unsigned numPagesRequired, SlotList *pSlotList) {
    if (m_numPagesCurrent < numPagesRequired) {
        ValueObject::s_pMemory->Show("PageList::AllocatePages");
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