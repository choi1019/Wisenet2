#include <03Technical/MemoryManager/SlotListGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <03Technical/MemoryManager/SlotList.h>

PageIndex *SlotListGenerator::s_pPageIndexHead = nullptr;

void* SlotListGenerator::operator new(size_t szThis, void* pMemoryAllocated, const char* sMessage) {
    s_pPageIndexHead = nullptr;
    return pMemoryAllocated;
}

void SlotListGenerator::operator delete(void* pObject) {
}

void SlotListGenerator::operator delete(void* pObject, void* pMemoryAllocated, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotListGenerator::delete", "_eNotSupport");
}

SlotListGenerator::SlotListGenerator(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
{ 
}
SlotListGenerator::~SlotListGenerator() {
}
void SlotListGenerator::Initialize() {
    MemoryObject::Initialize();
}
void SlotListGenerator::Finalize() {
    MemoryObject::Finalize();
}
//------------------------------------------------------------------------------
void *SlotListGenerator::AllocateAPage() {
    PageIndex *pPageIndex = SlotList::s_pPageList->AllocatePages(1, nullptr);
    pPageIndex->AllocateSlots(sizeof(SlotList));
    pPageIndex->SetPSibling(s_pPageIndexHead);
    s_pPageIndexHead = pPageIndex;

    void* pSlotListChunk = pPageIndex->AllocateASlot();
    return pSlotListChunk;
}

void* SlotListGenerator::Malloc(size_t szObject, const char* sMessage) {
    void *pSlotListChunk = nullptr;   
    PageIndex *pPageIndex = s_pPageIndexHead;
    while (pPageIndex != nullptr) {
        pSlotListChunk = pPageIndex->AllocateASlot();
        if (pSlotListChunk != nullptr) {
            return pSlotListChunk;
        }
        pPageIndex = pPageIndex->GetPSibling();
    }
    if (pSlotListChunk == nullptr) {
        pSlotListChunk = AllocateAPage();
    }
    return pSlotListChunk; 
}

bool SlotListGenerator::Free(void* pObject) {
    int idxPage =  SlotList::s_pPageList->GetIdxPage(pObject);
    PageIndex *pPrevious = nullptr;
    PageIndex *pCurrent = s_pPageIndexHead;
    while (pCurrent != nullptr) {
        // found
        if (pCurrent->GetIndex() == idxPage) {
            pCurrent->DelocateASlot(pObject);
            if (pCurrent->IsGarbage()) {
                // head page is garbase
                if (pPrevious == nullptr) {
                    s_pPageIndexHead = s_pPageIndexHead->GetPSibling(); 
                } else {
                    pPrevious->SetPSibling(pCurrent->GetPSibling());
                }
                SlotList::s_pPageList->DelocatePages(pCurrent->GetIndex()); ;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    return false;
}

// maintenance
void SlotListGenerator::Show(const char* sMessage) { 
};