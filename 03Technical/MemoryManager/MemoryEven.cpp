#include <03Technical/MemoryManager/MemoryEven.h>
#include <03Technical/MemoryManager/SlotInfo.h>
#include <01Base/Aspect/Log.h>

// for head MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int nClassId, const char* pClassName)
    : SlotList(szSlot, nClassId, pClassName) 
    {
    }
// for normal MemoryEven
MemoryEven::MemoryEven(size_t szSlot, int numMaxSlots, int numPagesRequired, SlotList *pSlotListHead,
            int nClassId,     const char* pClassName)
    : SlotList(szSlot, numMaxSlots, numPagesRequired, pSlotListHead, nClassId, pClassName) 
    {
    }
MemoryEven::~MemoryEven(){}

void MemoryEven::Initialize() {
    SlotList::Initialize();
}
void MemoryEven::Finalize() {
    SlotList::Finalize();
}

void MemoryEven::AddSlotInfo(Slot *pSlot, const char *sMessage) {
    SlotInfo *pSlotInfo = new("SlotInfo") SlotInfo(pSlot, sMessage, this);
    pSlotInfo->SetPNext(m_pSlotInfoHead);
    m_pSlotInfoHead = pSlotInfo;
    MLOG_NEWLINE("AddSlotInfo", (size_t)pSlot, sMessage);
}

void MemoryEven::DeleteSlotInfo(Slot *pSlot) {
    MemoryEven *pSibling = this;
    SlotInfo *pPrevious = nullptr;
    SlotInfo *pCurrent = m_pSlotInfoHead;
    while (pCurrent != nullptr) {
        if (pCurrent->GetPSlot() == pSlot) {
            if (pPrevious == nullptr) {
                m_pSlotInfoHead = pCurrent->GetPNext();
            } else {
                pPrevious->SetPNext(pCurrent->GetPNext());
            }
            MLOG_NEWLINE("DeleteSlotInfo", (size_t)pSlot, pCurrent->GetSMessage());
            delete pCurrent;
            return;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    throw EXCEPTION(-1);
}

SlotInfo *MemoryEven::GetPSlotInfo(Slot *pSlot) {
    SlotInfo *pSlotInfo = m_pSlotInfoHead;
    while (pSlotInfo != nullptr) {
        if (pSlotInfo->GetPSlot() == pSlot) {
            return pSlotInfo;
        }
    }
    return nullptr;
}

void* MemoryEven::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    this->AddSlotInfo((Slot *)pMemoryAllocated, pcName);
    UnLock();
    return pMemoryAllocated;
}

bool MemoryEven::SafeFree(void* pObject) {
    Lock();
    bool result = this->Free(pObject);
    this->DeleteSlotInfo((Slot *)pObject);
    UnLock();
    return result;
}

// maintenance
void MemoryEven::Show(const char* pTitle) {
    SlotList::Show(pTitle);
    SlotInfo *pSlotInfo = this->m_pSlotInfoHead;
    while (pSlotInfo != nullptr) {
        pSlotInfo->Show("");
        pSlotInfo = pSlotInfo->GetPNext();
    }
}
