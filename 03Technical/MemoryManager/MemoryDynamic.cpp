#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <math.h>
#include <02Platform/EventQueue/Event.h>
#include <03Technical/MemoryManager/SlotInfo.h>

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
    this->m_pSlotListHead = new("MemoryDynamic::m_pSlotListHead") SlotList(0);
    this->m_szUnitExponentOf2 = (unsigned)(log2(static_cast<double>(this->m_szUnit))); 
}

MemoryDynamic::~MemoryDynamic() {
}

void MemoryDynamic::Initialize() {
    MemoryObject::Initialize();
}
void MemoryDynamic::Finalize() {
    MemoryObject::Finalize();
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
            Slot *pTargetSlot = (Slot *)pTargetSlotList->SafeMalloc(szSlot, sMessage);
            return pTargetSlot;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed, " MemoryDynamic::Malloc", __LINE__);
}

bool MemoryDynamic::Free(void* pObject) {
    SlotList *pPrevious = m_pSlotListHead; 
    SlotList *pCurrent = pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        bool bFreed = pCurrent->SafeFree(pObject);
        if (bFreed) {
            // delete size head?
            if (pCurrent->GetCountSlotLists() == 0) {
                pPrevious->SetPNext(pCurrent->GetPNext());
                delete pCurrent;
            }
            return true;
        }
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic::Free", __LINE__);
    return false;
}

void* MemoryDynamic::SafeMalloc(size_t szAllocate, const char* pcName)
{
    Lock();
    void* pMemoryAllocated = this->Malloc(szAllocate, pcName);
    UnLock();
    return pMemoryAllocated;
}
bool MemoryDynamic::SafeFree(void* pObject) {
    Lock();
    bool result = this->Free(pObject);
    UnLock();
    return result;
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
    Event::s_pMemory->Show("Event");
    SlotInfo::s_pMemory->Show("SlotInfo");
    MLOG_FOOTER("MemoryDynamic::Show");
};