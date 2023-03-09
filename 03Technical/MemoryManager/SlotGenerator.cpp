#include <03Technical/MemoryManager/SlotGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>

//------------------------------------------------------------------------------
PageList *SlotGenerator::s_pPageList;

void* SlotGenerator::operator new(size_t szThis, void* pMemoryAllocated, const char* sMessage) {
    return pMemoryAllocated;
}
void SlotGenerator::operator delete(void* pObject) {    
}
void SlotGenerator::operator delete(void* pObject, void* PMemoryAllocated, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotGenerator::delete", "_eNotSupport");
}

//------------------------------------------------------------------------------
SlotGenerator::SlotGenerator(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_pPageIndexHead(nullptr)
{ 
}
SlotGenerator::~SlotGenerator() {
}
void SlotGenerator::Initialize() {
    MemoryObject::Initialize();
}
void SlotGenerator::Finalize() {
    MemoryObject::Finalize();
}

//------------------------------------------------------------------------------
PageIndex *SlotGenerator::AllocateAPage(size_t szSlot) {
    PageIndex *pPageIndex = s_pPageList->AllocatePages(1, nullptr);
    pPageIndex->AllocateSlots(szSlot);
    pPageIndex->SetPSibling(m_pPageIndexHead);
    m_pPageIndexHead = pPageIndex;
    return pPageIndex;
}

void* SlotGenerator::Malloc(size_t szObject, const char* sMessage) {
    void *pSlotChunk = nullptr;   
    PageIndex *pPageIndex = m_pPageIndexHead;
    while (pPageIndex != nullptr) {
        pSlotChunk = pPageIndex->AllocateASlot();
        if (pSlotChunk != nullptr) {
            return pSlotChunk;
        }
        pPageIndex = pPageIndex->GetPSibling();
    }
    if (pSlotChunk == nullptr) {
        pPageIndex = AllocateAPage(szObject);
        pSlotChunk = pPageIndex->AllocateASlot();
    }
    return pSlotChunk; 
}

//------------------------------------------------------------------------------
bool SlotGenerator::Free(void* pObject) {
    int idxPage =  s_pPageList->GetIdxPage(pObject);
    PageIndex *pPrevious = nullptr;
    PageIndex *pCurrent = m_pPageIndexHead;
    while (pCurrent != nullptr) {
        // found
        if (pCurrent->GetIndex() == idxPage) {
            pCurrent->DelocateASlot(pObject);
            if (pCurrent->IsGarbage()) {
                // head page is garbase
                if (pPrevious == nullptr) {
                    m_pPageIndexHead = m_pPageIndexHead->GetPSibling(); 
                } else {
                    pPrevious->SetPSibling(pCurrent->GetPSibling());
                }
                s_pPageList->DelocatePages(pCurrent->GetIndex()); ;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotListDelocationFailed, "SlotGenerator::Free", "Free");
}
//------------------------------------------------------------------------------
// maintenance
void SlotGenerator::Show(const char* sMessage) {
    // PageIndex *pPageIndex = s_pPageIndexHead;
    // while (pPageIndex != nullptr) {
    //     PageIndex::Slot *pSlot = (PageIndex::Slot*)pPageIndex->GetPSlotHead();
    //     while (pSlot != nullptr) {
    //         pSlot->Show(sMessage);
    //         pSlot = pSlot->GetPNext();
    //     }
    //     pPageIndex = pPageIndex->GetPSibling();
    // }
};