#include <13PTechnical/PMemoryManager/PMemoryManager.h>

#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

// #include <13PTechnical/PMemoryManager/PMemoryEven.h>
#include <02Platform/EventQueue/Event.h>
#include <03Technical/MemoryManager/SlotInfo.h>
#include <01Base/Object/BaseObject.h>

// sizes
size_t PMemoryManager::s_szSystemMemory = 0;
size_t PMemoryManager::s_szApplicationMemory = 0;
size_t PMemoryManager::s_szPage = 0;
size_t PMemoryManager::s_szSlotUnit = 0;
// physical memory
char* PMemoryManager::s_pSystemMemeory = nullptr;
char* PMemoryManager::s_pApplicationMemeory = nullptr;;
// memory manager
PMemoryStatic* PMemoryManager::s_pMemoryStatic = nullptr;
PageList *PMemoryManager::s_pPageList = nullptr;
PMemoryDynamic* PMemoryManager::s_pMemoryDynamic = nullptr;

void PMemoryManager::Allocate(size_t szSystemMemory, size_t szApplicationMemory,  size_t szPage, size_t szSlotUnit) {
    PMemoryManager::s_szSystemMemory = szSystemMemory;
    PMemoryManager::s_szApplicationMemory = szApplicationMemory;
    PMemoryManager::s_szPage = szPage;
    PMemoryManager::s_szSlotUnit = szSlotUnit;

    try {
        MLOG_HEADER("PMemoryManager::Allocate");
            // system memory
            // PMemoryManager::s_pSystemMemeory = new char[s_szSystemMemory];
            // PMemoryManager::s_pMemoryStatic = new(s_pSystemMemeory, s_szSystemMemory, "PMemoryStatic") PMemoryStatic();
            // PMemoryManager::s_pMemoryStatic->Initialize();
            // BaseObject::s_pMemory = s_pMemoryStatic;
            // SHOW_STATIC("PMemoryManager::Allocate");

            // aplication memory
            PMemoryManager::s_pApplicationMemeory = new char[s_szApplicationMemory];
            PMemoryManager::s_pMemoryDynamic = new(s_pApplicationMemeory, s_szApplicationMemory, s_szPage, "PMemoryDynamic") PMemoryDynamic(s_szSlotUnit);
            PMemoryManager::s_pMemoryDynamic->Initialize();
            ValueObject::s_pMemory = s_pMemoryDynamic;
            /////////////////////////////////////////
            BaseObject::s_pMemory = s_pMemoryDynamic;

            // memory even            
            // Event::s_pMemory = new("Event::s_pMemory") PMemoryEven(sizeof(Event));
            
            SHOW_DYNAMIC("PMemoryDynamic::Allocate");

        MLOG_FOOTER("PMemoryManager::Allocate");
    } catch (Exception& exception) {
        exception.Println();
    }
}

void PMemoryManager::Delocate() {
    try {
        MLOG_HEADER("PMemoryManager::Delocate");

            s_pMemoryDynamic->Finalize();
            SHOW_DYNAMIC("PMemoryDynamic::Delocate");
            SHOW_STATIC("PMemoryManager::Delocate");
            delete s_pMemoryDynamic;

            delete s_pPageList;
            delete[] s_pApplicationMemeory;

            // s_pMemoryStatic->Finalize();
            // delete s_pMemoryStatic;
            // delete[] s_pSystemMemeory;

        MLOG_FOOTER("PMemoryManager::Delocate");
    } catch (Exception& exception) {

    }
}