#include <03Technical/MemoryManager/MemoryDynamic.h>
#include <03Technical/MemoryManager/SlotInfo.h>

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <math.h>
#include <02Platform/EventQueue/Event.h>

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
    // pages to share among SlotLists, MemoryEvents and PMemoryEvents
    SlotList::s_pPageList = MemoryDynamic::s_pPageList;
    // set the memory manager of a SlotInfo as a SlotList
    SlotInfo::s_pMemory = new("SlotList-SlotInfo") SlotList(sizeof(SlotInfo));

    // create a head SlotList
    this->m_pMemoryEvenHead = new("MemoryEven-Head") MemoryEven(0);
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
    // find the MemoryEven
    MemoryEven *pTargetMemoryEven = nullptr;
    MemoryEven *pPrevious = nullptr;
    MemoryEven *pCurrent = m_pMemoryEvenHead;
    while (pCurrent != nullptr) {
        if (pCurrent->GetSzSlot() == szSlot) {
            // arleady exist
            pTargetMemoryEven = pCurrent;
        } else if (pCurrent->GetSzSlot() < szSlot) {
            if (pCurrent->GetPNext() == nullptr) {
                // add a new MemoryEven
                pTargetMemoryEven = new("MemoryEvenHead") MemoryEven(szSlot);
                pCurrent->SetPNext(pTargetMemoryEven);
                pTargetMemoryEven->SetPNext(nullptr);
            }
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // insert a new MemoryEven
            pTargetMemoryEven = new("MemoryEvenHead") MemoryEven(szSlot);
            pPrevious->SetPNext(pTargetMemoryEven);
            pTargetMemoryEven->SetPNext(pCurrent);
        }
        // found
        if (pTargetMemoryEven != nullptr) {
            Slot *pTargetSlot = (Slot *)pTargetMemoryEven->Malloc(szSlot, sMessage);
            // return generated memory
            return pTargetSlot;
        }
        pPrevious = pCurrent;
        pCurrent = (MemoryEven *)pCurrent->GetPNext();
    } 
    throw Exception((unsigned)IMemory::EException::_eSlotlistAllocationFailed, " MemoryDynamic::Malloc", __LINE__);
}

bool MemoryDynamic::Free(void* pObject) {
    MemoryEven *pPrevious = m_pMemoryEvenHead; 
    MemoryEven *pCurrent = (MemoryEven *)pPrevious->GetPNext(); 
    while (pCurrent != nullptr) {
        bool bFreed = pCurrent->Free(pObject);
        if (bFreed) {
            // delete size head
            /////// ????
            // if (pCurrent->GetCountSlotLists() == 0) {
            //     pPrevious->SetPNext(pCurrent->GetPNext());
            //     delete pCurrent;
            // }
            // delete
            return true;
        }
        pCurrent = (MemoryEven *)pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eSlotlistFreeFailed, "MemoryDynamic::Free", __LINE__);
    return false;
}

// maintenance
void MemoryDynamic::Show(const char* pTitle) {
    MLOG_HEADER("MemoryDynamic::Show(pTitle)", pTitle);
    s_pPageList->Show("MemoryDynamic::Show");
    MemoryEven* pMemoryEven = this->m_pMemoryEvenHead;
    while (pMemoryEven != nullptr) {
        pMemoryEven->Show("Head");
        pMemoryEven = (MemoryEven *)pMemoryEven->GetPNext();
    }
    SlotInfo::s_pMemory->Show("SlotInfo");
    Event::s_pMemory->Show("Event");
    MLOG_FOOTER("MemoryDynamic::Show");
};