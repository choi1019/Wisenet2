#include <03Technical/MemoryManager/PageIndex.h>
#include <03Technical/MemoryManager/SlotList.h>

#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

void* PageIndex::operator new(size_t szThis, void *pMemoryAllocated, const char* sMessage) {
    return pMemoryAllocated;
}
void PageIndex::operator delete(void* pObject) {
    // delete this
 }
void PageIndex::operator delete(void* pObject, void *pMemoryAllocated, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "PageIndex::delete", __LINE__);
}

PageIndex::PageIndex(
    int nClassId,
    const char* pClassName)
    : MemoryObject(nClassId, pClassName)		
    , m_index(0)
    , m_pPage(nullptr)

    , m_bAllocated(false)
    , m_numAllocated(1)
    , m_pSlotList(nullptr)
    , m_numSlotsAllocated(0)
    , m_numSlotsCurrent(0)
    , m_pSlotHead(nullptr)
{
}

PageIndex::~PageIndex() {
}

void PageIndex::Initialize(unsigned indexPage, void* pPage) {
    MemoryObject::Initialize();
    m_index = indexPage;
    m_pPage = (Page*)pPage;
}

void PageIndex::Finalize() {
    MemoryObject::Finalize();
}

void PageIndex::Show(const char* pTitle) {
    MLOG_NEWLINE("PageIndex::show(index, pPage, numAllocated, bAllocated)"
                        , m_index, (size_t)m_pPage, m_numAllocated, m_bAllocated);
}

void PageIndex::AllocateSlots(int szSlot) {
    int szPage = SlotInfo::s_pPageList->GetSzPage();
    if (szSlot > szPage) {
        throw Exception(-1, "AllocateSlots");
    } else {
        m_numSlotsAllocated = szPage / szSlot;
        m_numSlotsCurrent = m_numSlotsAllocated;
        
        m_pSlotHead = (Slot*)this->GetPPage();
        Slot* pCurrent = m_pSlotHead;
        for (int i = 1; i < m_numSlotsAllocated; i++) {
            pCurrent->m_pNext = (Slot*)((size_t)pCurrent + szSlot);
            pCurrent = pCurrent->m_pNext;
        }
        pCurrent->m_pNext = nullptr;

    }
}
void *PageIndex::AllocateASlot() { 
    void *pSlot = nullptr;
    if (m_pSlotHead != nullptr) {
        pSlot = m_pSlotHead;
        m_pSlotHead = m_pSlotHead->m_pNext;
        --m_numSlotsCurrent; 
    }
    return (void*)pSlot;
}
void PageIndex::DelocateASlot(void *pObject) { 
    Slot *pSlot = (Slot*)pObject;
    pSlot->m_pNext = nullptr;
    if (m_pSlotHead == nullptr) {
        m_pSlotHead = pSlot;
    } else {
        pSlot->m_pNext = m_pSlotHead;
        m_pSlotHead = pSlot;
    }
    ++m_numSlotsCurrent;
}