#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>
#include <01Base/Aspect/LogMemory.h>

#include <math.h>

PageList* MemoryDynamic::s_pPageList = nullptr;

void* MemoryDynamic::operator new(size_t szThis, const char* sMessage) {
    void* pMemoryDynamic = BaseObject::s_pMemory->SafeMalloc(szThis, sMessage);    
    SlotList::s_pSlotListRecycle = nullptr;
    return pMemoryDynamic;
}
void MemoryDynamic::operator delete(void* pObject) {
    MemoryObject::s_pMemory->SafeFree(pObject);
 }
void MemoryDynamic::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryDynamic::delete", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// constructors and destructors
MemoryDynamic::MemoryDynamic(unsigned szSlotUnit, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szUnit(szSlotUnit)
{
    // set memory manager of ValueObject as this
    ValueObject::s_pMemory = this;
    SlotList::s_pPageList = MemoryDynamic::s_pPageList;

    this->m_pSlotListHead = new("MemoryDynamic::m_pSlotListHead") SlotList(0);
    this->m_szUnitExponentOf2 = (unsigned)(log2(static_cast<double>(this->m_szUnit))); 

    SlotList::s_aPSlotList = (SlotList **) BaseObject::s_pMemory->SafeMalloc(sizeof(SlotList *) * MemoryDynamic::s_pPageList->GetNumPagesMax(), "pSlotList Array");
}

MemoryDynamic::~MemoryDynamic() {
}

void MemoryDynamic::Initialize() {
    MemoryObject::Initialize();
    SHOW_DYNAMIC("MemoryDynamic::Initialize");
}
void MemoryDynamic::Finalize() {
    MemoryObject::Finalize();
    SHOW_DYNAMIC("MemoryDynamic::~Finalize");
}

// methods
void* MemoryDynamic::Malloc(size_t szObject, const char* sMessage) {
     // compute slot size
    size_t szSlot = szObject;
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }
    // find the slotList
    SlotList *pTargetSlotList = nullptr;
    SlotList *pPrevious = nullptr;
    SlotList *pCurrent = m_pSlotListHead; 
    while (pCurrent != nullptr) {
        if (pCurrent->GetSzSlot() == szSlot) {
            // arleady exist
            pTargetSlotList = pCurrent;
        } else if (pCurrent->GetSzSlot() < szSlot) {
            if (pCurrent->GetPNext() == nullptr) {
                // add a new SlotList
                pTargetSlotList = new("SlotListHead") SlotList(szSlot);
                pCurrent->SetPNext(pTargetSlotList);
                pTargetSlotList->SetPNext(nullptr);
            }
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // insert a new SlotList
            pTargetSlotList = new("SlotListHead") SlotList(szSlot);
            pPrevious->SetPNext(pTargetSlotList);
            pTargetSlotList->SetPNext(pCurrent);
        }
        // found
        if (pTargetSlotList != nullptr) {
            Slot *pTargetSlot = pTargetSlotList->AllocateASlot(sMessage);
            return pTargetSlot;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed, " MemoryDynamic::Malloc", __LINE__);
}

void MemoryDynamic::Free(void* pObject) {
    SlotList *pPrevious = m_pSlotListHead; 
    SlotList *pCurrent = pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        bool bFreed = pCurrent->FreeASlot((Slot *)pObject);
        if (bFreed) {
            // delete size head?
            if (pCurrent->GetCountSlotLists() == 0) {
                pPrevious->SetPNext(pCurrent->GetPNext());
                delete pCurrent;
            }
            return;
        }
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic::Free", __LINE__);
}

void* MemoryDynamic::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}
void MemoryDynamic::SafeFree(void* pObject) {
    Lock();
    this->Free(pObject);
    UnLock();
}

// maintenance
void MemoryDynamic::Show(const char* pTitle) {
    MLOG_HEADER("MemoryDynamic::Show(pTitle)", pTitle);
    s_pPageList->Show("MemoryDynamic::Show");
    SlotList* pSlotList = this->m_pSlotListHead;
    while (pSlotList != nullptr) {
        pSlotList->Show("Head");
        pSlotList = pSlotList->GetPNext();
    }
    MLOG_FOOTER("MemoryDynamic::Show");
};