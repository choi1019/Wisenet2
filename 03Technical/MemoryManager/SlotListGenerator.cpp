#include <03Technical/MemoryManager/SlotListGenerator.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/Exception.h>
#include <stdlib.h>
#include <03Technical/MemoryManager/SlotList.h>

SlotListChunk *SlotListGenerator::s_pSlotLisChunktHead = nullptr;

void* SlotListGenerator::operator new(size_t szThis, void* pMemoryAllocated, const char* sMessage) {
    SlotListChunk *pSlotListChunk = nullptr;
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

void SlotListGenerator::GenerateSlotListChunks() {
    Page *pPage = SlotList::s_pPageList->AllocatePages(1, nullptr)->GetPPage();
    int szSlotList = sizeof(SlotList);
    int szPage = SlotList::s_pPageList->GetSzPage();
    int numMaxSlotListChunks = szPage / szSlotList;

    s_pSlotLisChunktHead = (SlotListChunk*)pPage;
    SlotListChunk* pPrevious = nullptr;
    SlotListChunk* pCurrent = s_pSlotLisChunktHead;
    for (int i = 0; i < numMaxSlotListChunks; i++) {
        pCurrent->pNext = (SlotListChunk*)((size_t)pCurrent + szSlotList);
        pPrevious = pCurrent;
        pCurrent = pCurrent->pNext;
    }
    pPrevious->pNext = nullptr;
}

void* SlotListGenerator::Malloc(size_t szObject, const char* sMessage) {
    if (s_pSlotLisChunktHead == nullptr) {        
        GenerateSlotListChunks();
    }
    SlotListChunk *pSlotListChunk = s_pSlotLisChunktHead;
    s_pSlotLisChunktHead = s_pSlotLisChunktHead->pNext;
    return pSlotListChunk; 
}

bool SlotListGenerator::Free(void* pObject) {
    ((SlotListChunk*)pObject)->pNext = s_pSlotLisChunktHead;
    s_pSlotLisChunktHead =  (SlotListChunk*)pObject;
    return true;
}

// maintenance
void SlotListGenerator::Show(const char* sMessage) { 
};