#include <03Technical/MemoryManager/SlotInfoGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <stdlib.h>
#include <03Technical/MemoryManager/SlotInfo.h>

//------------------------------------------------------------------------------
SlotInfoChunk *SlotInfoGenerator::s_pSlotInfoChunktHead = nullptr;

void* SlotInfoGenerator::operator new(size_t szThis, void* PMemoryAllocated, const char* sMessage) {
    s_pSlotInfoChunktHead = nullptr;
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
    Page *pPage = SlotInfo::s_pPageList->AllocatePages(1, nullptr)->GetPPage();
    int szSlotInfo = sizeof(SlotInfo);
    int szPage = SlotInfo::s_pPageList->GetSzPage();
    int numMaxSlotInfoChunks = szPage / szSlotInfo;

    s_pSlotInfoChunktHead = (SlotInfoChunk*)pPage;
    SlotInfoChunk* pPrevious = nullptr;
    SlotInfoChunk* pCurrent = s_pSlotInfoChunktHead;
    for (int i = 0; i < numMaxSlotInfoChunks; i++) {
        pCurrent->pNext = (SlotInfoChunk*)((size_t)pCurrent + szSlotInfo);
        pPrevious = pCurrent;
        pCurrent = pCurrent->pNext;
    }
    pPrevious->pNext = nullptr;
}

void* SlotInfoGenerator::Malloc(size_t szObject, const char* sMessage) {
    if (s_pSlotInfoChunktHead == nullptr) {        
        GenerateSlotInfoChunks();
    }
    SlotInfoChunk *pSlotInfoChunk = s_pSlotInfoChunktHead;
    s_pSlotInfoChunktHead = s_pSlotInfoChunktHead->pNext;
    return pSlotInfoChunk; 
}
//------------------------------------------------------------------------------
bool SlotInfoGenerator::Free(void* pObject) {
    ((SlotInfoChunk*)pObject)->pNext = s_pSlotInfoChunktHead;
    s_pSlotInfoChunktHead =  (SlotInfoChunk*)pObject;
    return true;
}
//------------------------------------------------------------------------------
// maintenance
void SlotInfoGenerator::Show(const char* sMessage) { 
};