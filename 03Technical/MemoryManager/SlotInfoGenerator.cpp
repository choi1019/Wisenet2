#include <03Technical/MemoryManager/SlotInfoGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <03Technical/MemoryManager/SlotInfo.h>

//------------------------------------------------------------------------------
PageIndex *SlotInfoGenerator::s_pPageIndexHead = nullptr;

void* SlotInfoGenerator::operator new(size_t szThis, void* pMemoryAllocated, const char* sMessage) {
    s_pPageIndexHead = nullptr;
    return pMemoryAllocated;
}
void SlotInfoGenerator::operator delete(void* pObject) {    
}
void SlotInfoGenerator::operator delete(void* pObject, void* PMemoryAllocated, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "SlotInfoGenerator::delete", "_eNotSupport");
}

//------------------------------------------------------------------------------
SlotInfoGenerator::SlotInfoGenerator(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
{ 
}
SlotInfoGenerator::~SlotInfoGenerator() {
}
void SlotInfoGenerator::Initialize() {
    MemoryObject::Initialize();
}
void SlotInfoGenerator::Finalize() {
    MemoryObject::Finalize();
}

//------------------------------------------------------------------------------
void *SlotInfoGenerator::AllocateAPage() {
    PageIndex *pPageIndex = SlotInfo::s_pPageList->AllocatePages(1, nullptr);
    pPageIndex->AllocateSlots(sizeof(SlotInfo));
    pPageIndex->SetPSibling(s_pPageIndexHead);
    s_pPageIndexHead = pPageIndex;

    void* pSlotInfoChunk = pPageIndex->AllocateASlot();
    return pSlotInfoChunk;
}

void* SlotInfoGenerator::Malloc(size_t szObject, const char* sMessage) {
    void *pSlotInfoChunk = nullptr;   
    PageIndex *pPageIndex = s_pPageIndexHead;
    while (pPageIndex != nullptr) {
        pSlotInfoChunk = pPageIndex->AllocateASlot();
        if (pSlotInfoChunk != nullptr) {
            return pSlotInfoChunk;
        }
        pPageIndex = pPageIndex->GetPSibling();
    }
    if (pSlotInfoChunk == nullptr) {
        pSlotInfoChunk = AllocateAPage();
    }
    return pSlotInfoChunk; 
}

//------------------------------------------------------------------------------
bool SlotInfoGenerator::Free(void* pObject) {
    int idxPage =  SlotInfo::s_pPageList->GetIdxPage(pObject);
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
                SlotInfo::s_pPageList->DelocatePages(pCurrent->GetIndex()); ;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPSibling();
    }
    return false;
}
//------------------------------------------------------------------------------
// maintenance
void SlotInfoGenerator::Show(const char* sMessage) { 
};