#include <03Technical/MemoryManager/SlotInfoGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <stdlib.h>
#include <03Technical/MemoryManager/SlotInfo.h>

//------------------------------------------------------------------------------
//SlotInfoChunk *SlotInfoGenerator::s_pSlotInfoChunktHead = nullptr;
PageIndex *SlotInfoGenerator::s_pPageIndexHead = nullptr;

void* SlotInfoGenerator::operator new(size_t szThis, void* PMemoryAllocated, const char* sMessage) {
    s_pPageIndexHead = nullptr;
    return PMemoryAllocated;
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
void SlotInfoGenerator::GenerateSlotInfoChunks() {
    PageIndex *pPageIndex = SlotInfo::s_pPageList->AllocatePages(1, nullptr);
    pPageIndex->GenerateSlots(sizeof(SlotInfo));
    pPageIndex->SetPSibling(s_pPageIndexHead);
    s_pPageIndexHead->SetPSibling(pPageIndex);
}

void* SlotInfoGenerator::Malloc(size_t szObject, const char* sMessage) {
    if (s_pSlotInfoChunktHead == nullptr) {        
        GenerateSlotInfoChunks();
    }
    SlotInfoChunk *pSlotInfoChunk = s_pSlotInfoChunktHead;
    s_pSlotInfoChunktHead = s_pSlotInfoChunktHead->pNext;

    // for SlotInfo
    // PageIndex *pPageIndex = SlotInfo::s_pPageList->GetPPageIndex(pSlotInfoChunk);
    // pPageIndex->AllocateASlot();

    return pSlotInfoChunk; 
}
//------------------------------------------------------------------------------
bool SlotInfoGenerator::Free(void* pObject) {
    ((SlotInfoChunk*)pObject)->pNext = s_pSlotInfoChunktHead;
    s_pSlotInfoChunktHead =  (SlotInfoChunk*)pObject;

    // for SlotInfo
    // PageIndex *pPageIndex = SlotInfo::s_pPageList->GetPPageIndex(pObject);
    // pPageIndex->DelocateASlot();
    // if (pPageIndex->IsGarbage()) {
    //     SlotInfo::s_pPageList->DelocatePages(pPageIndex->GetIndex());
    // }

    return true;
}
//------------------------------------------------------------------------------
// maintenance
void SlotInfoGenerator::Show(const char* sMessage) { 
};