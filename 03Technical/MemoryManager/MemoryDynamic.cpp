#include <03Technical/MemoryManager/MemoryDynamic.h>

#include <03Technical/MemoryManager/SlotInfoGenerator.h>
#include <03Technical/MemoryManager/SlotInfo.h>

#include <03Technical/MemoryManager/SlotList.h>
#include <03Technical/MemoryManager/SlotListGenerator.h>

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

#include <math.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////
// static

void* MemoryDynamic::s_pMemoryAllocated = nullptr;
size_t MemoryDynamic::s_szMemoryAllocated = 0;
void* MemoryDynamic::s_pMemoryCurrent = nullptr;
size_t MemoryDynamic::s_szMemoryCurrent = 0;

PageList* MemoryDynamic::s_pPageList = nullptr;

void* MemoryDynamic::operator new(size_t szThis, void* pMemoryAllocated, size_t szMemoryAllocated, const char* sMessage) {
    s_pMemoryAllocated = pMemoryAllocated;
    s_szMemoryAllocated = szMemoryAllocated;
    // allocate this
    s_pMemoryCurrent = (void*)((size_t)s_pMemoryAllocated + szThis);
    s_szMemoryCurrent = s_szMemoryAllocated - szThis;
    
    return s_pMemoryAllocated;
}
void MemoryDynamic::operator delete(void* pObject) {
    // delete this    
 }
void MemoryDynamic::operator delete(void* pObject, void *pApplicationMemeory, size_t szApplicationMemory, const char* sMessage) {
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryDynamic::delete", __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

MemoryDynamic::MemoryDynamic(int nClassId, const char* pClassName)
    : MemoryObject(nClassId, pClassName)
    , m_szPage(0)
    , m_szUnit(0)
    , m_pSlotListHead(nullptr)
    , m_szUnitExponentOf2(0)
{
}
MemoryDynamic::~MemoryDynamic() {
}

void MemoryDynamic::Initialize(int szPage, int szSlotUnit) {
    MemoryObject::Initialize();
    m_szPage = szPage;
    m_szUnit = szSlotUnit;
    m_szUnitExponentOf2 = (unsigned)(log2(static_cast<double>(this->m_szUnit)));

    // PageList
    s_pPageList = new(s_pMemoryCurrent, "PageList") PageList();
        s_pMemoryCurrent = (void *)((size_t)s_pMemoryCurrent + sizeof(PageList));
        s_szMemoryCurrent = s_szMemoryAllocated - sizeof(PageList);
    // SlotListGenerator
    SlotList::s_pMemory = new(s_pMemoryCurrent, "SlotListGenerator") SlotListGenerator();
        s_pMemoryCurrent = (void *)((size_t)s_pMemoryCurrent + sizeof(SlotListGenerator));
        s_szMemoryCurrent = s_szMemoryAllocated - sizeof(SlotListGenerator);
    // SlotInfoGenerator
    SlotInfo::s_pMemory = new(s_pMemoryCurrent, "SlotInfoGenerator") SlotInfoGenerator();
        s_pMemoryCurrent = (void *)((size_t)s_pMemoryCurrent + sizeof(SlotInfoGenerator));
        s_szMemoryCurrent = s_szMemoryAllocated - sizeof(SlotInfoGenerator);   

    // PageIndex, Page
    size_t szAllocated = s_pPageList->Initialize(s_pMemoryCurrent, s_szMemoryCurrent, szPage);
        s_pMemoryCurrent = (void *)((size_t)s_pMemoryCurrent + szAllocated);
        s_szMemoryCurrent = s_szMemoryCurrent - szAllocated;

    // associate
    SlotList::s_pPageList = MemoryDynamic::s_pPageList;
    SlotInfo::s_pPageList = MemoryDynamic::s_pPageList;

    // create a head SlotList
    this->m_pSlotListHead = new("SlotList-Head") SlotList(0, nullptr);
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
        pCurrent = pCurrent->GetPNext();
    }
    throw Exception((unsigned)IMemory::EException::_eNotSupport, "MemoryDynamic::GetPSlotInfo", __LINE__);
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
    SlotList *pPrevious = m_pSlotListHead;
    SlotList *pCurrent = pPrevious->GetPNext();
    while (pCurrent != nullptr) {
        if (pCurrent->GetSzSlot() == szSlot) {
            // arleady exist
            pTargetSlotList = pCurrent;
            break;    
        } else if (pCurrent->GetSzSlot() > szSlot) {
            // insert a new SlotList
            pTargetSlotList = new("SlotListHead") SlotList(szSlot, nullptr);
            pPrevious->SetPNext(pTargetSlotList);
            pTargetSlotList->SetPNext(pCurrent);
            break;
        }
        pPrevious = pCurrent;
        pCurrent = (SlotList *)pCurrent->GetPNext();
    }
    if (pTargetSlotList == nullptr) {
        // add a new SlotList at the end
        pTargetSlotList = new("SlotListHead") SlotList(szSlot, nullptr);
        pPrevious->SetPNext(pTargetSlotList);
    }
    Slot *pTargetSlot = (Slot *)pTargetSlotList->Malloc(szSlot, sMessage);
    return pTargetSlot;
}

bool MemoryDynamic::Free(void* pObject) {
    SlotList *pPrevious = m_pSlotListHead; 
    SlotList *pCurrent = pPrevious->GetPNext();
    while (pCurrent != nullptr) {
        bool bFreed = pCurrent->Free(pObject);
        if (bFreed) {
            if (pCurrent->GetCountSlotLists() == 0) {
                pPrevious->SetPNext(pCurrent->GetPNext());
                delete pCurrent;
            }
            return true;
        }
        pPrevious = pCurrent;
        pCurrent = pCurrent->GetPNext();
    }
    ValueObject::s_pMemory->Show("MemoryDynamic::Free");
    throw Exception((unsigned)IMemory::EException::_eSlotListDelocationFailed, "MemoryDynamic::Free", __LINE__);
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

    //    Event::s_pMemory->Show("Event");

    MLOG_FOOTER("MemoryDynamic::Show");
};