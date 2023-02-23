#include <03Technical/MemoryManager/PageIndex.h>
#include <03Technical/MemoryManager/SlotList.h>

#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

PageIndex::PageIndex(
    unsigned index,
    size_t pMemoryAllocated,
    int nClassId,
    const char* pClassName)
    : MemoryObject(nClassId, pClassName)		
    , m_index(index)
    , m_pPage((Page*)pMemoryAllocated)	
    , m_numAllocated(1)
    , m_bAllocated(false)
    , m_pSlotList(nullptr)
{
}

PageIndex::~PageIndex() {
}

void PageIndex::Initialize() {
    MemoryObject::Initialize();
}

void PageIndex::Finalize() {
    MemoryObject::Finalize();
}

void PageIndex::Show(const char* pTitle) {
    MLOG_NEWLINE("PageIndex::show(index, pPage, numAllocated, bAllocated)"
                        , m_index, (size_t)m_pPage, m_numAllocated, m_bAllocated);
}