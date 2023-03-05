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