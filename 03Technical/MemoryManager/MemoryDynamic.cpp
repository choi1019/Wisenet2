#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <03Technical/MemoryManager/SlotInfo.h>
#include <03Technical/MemoryManager/SlotList.h>
#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////
// static

PageList* MemoryDynamic::s_pPageList = nullptr;

void* MemoryDynamic::operator new(size_t szThis, const char* sMessage) {
    // generate this
    void* pMemoryDynamic = BaseObject::s_pMemory->Malloc(szThis, sMessage);    
    SlotList::s_pSlotListRecycle = nullptr;
    return pMemoryDynamic;
}
void MemoryDynamic::operator delete(void* pObject) {
    // delete this
    MemoryObject::s_pMemory->Free(pObject);
 }
void MemoryDynamic::operator delete(void* pObject, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryDynamic::delete", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryDynamic::MemoryDynamic(unsigned szSlotUnit, int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szUnit(szSlotUnit)
{
    // pages to share among SlotLists, SlotListts and PSlotListts
    SlotList::s_pPageList = MemoryDynamic::s_pPageList;
    // set the memory manager of a SlotInfo as a SlotList
    SlotInfo::s_pMemory = new("SlotList-SlotInfo") SlotList(sizeof(SlotInfo));

    // create a head SlotList
    this->m_pSlotListHead = new("SlotList-Head") SlotList(0);
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
SlotInfo *MemoryDynamic::GetPSlotInfo(void* pObject) {
    SlotList *pCurrent = m_pSlotListHead;
    while (pCurrent != nullptr) {
        SlotInfo *pSlotInfo = pCurrent->GetPSlotInfo(pObject);
        if (pSlotInfo != nullptr) {
            return pSlotInfo;
        }
        pCurrent = (SlotList *)(pCurrent->GetPNext());
    }
    return nullptr;
    // throw EXCEPTION(-1);
}

void* MemoryDynamic::Malloc(size_t szObject, const char* sMessage) {
     // compute slot size
    size_t szSlot = szObject;
    szSlot >>= m_szUnitExponentOf2;
    szSlot <<= m_szUnitExponentOf2;
    if (szSlot < szObject) {
        szSlot += m_szUnit;
    }
    // find the SlotList
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
            }
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // insert a new SlotList
            pTargetSlotList = new("SlotListHead") SlotList(szSlot);
            pPrevious->SetPNext(pTargetSlotList);
            pTargetSlotList->SetPNext(pCurrent);
        }
        // found
        if (pTargetSlotList != nullptr) {
            Slot *pTargetSlot = (Slot *)pTargetSlotList->Malloc(szSlot, sMessage);
            // return generated memory
            return pTargetSlot;
        }
        pPrevious = pCurrent;
        pCurrent = (SlotList *)pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed, " MemoryDynamic::Malloc", __LINE__);
}

bool MemoryDynamic::Free(void* pObject) {
    SlotList *pPrevious = m_pSlotListHead; 
    SlotList *pCurrent = (SlotList *)pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        bool bFreed = pCurrent->Free(pObject);
        if (bFreed) {
            // delete size head
            if (pCurrent->GetCountSlotLists() == 0) {
                pPrevious->SetPNext(pCurrent->GetPNext());
                delete pCurrent;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = (SlotList *)pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic::Free", __LINE__);
    return false;
}

// maintenance
void MemoryDynamic::Show(const char* pTitle) {
    MLOG_HEADER("MemoryDynamic::Show(pTitle)", pTitle);
        s_pPageList->Show("MemoryDynamic::Show");

        SlotList* pSlotList = this->m_pSlotListHead;
        while (pSlotList != nullptr) {
            pSlotList->Show("Head");
            pSlotList = (SlotList *)pSlotList->GetPNext();
        }

    //    SlotInfo::s_pMemory->Show("SlotInfo");
    //    Event::s_pMemory->Show("Event");

        // SlotListRecycle
        SlotList *pSlotListRecycle = SlotList::s_pSlotListRecycle;
        while (pSlotListRecycle != nullptr) {
            MLOG_NEWLINE("SlotListRecycle-", (size_t)pSlotListRecycle);
            pSlotListRecycle = pSlotListRecycle->GetPNext();
        }
    MLOG_FOOTER("MemoryDynamic::Show");
};