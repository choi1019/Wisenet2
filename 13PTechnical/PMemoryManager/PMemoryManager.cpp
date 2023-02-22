#include <13PTechnical/PMemoryManager/PMemoryManager.h>

#include <13PTechnical/PMemoryManager/Config.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

size_t PMemoryManager::s_szSystemMemory = SIZE_MEMORY_SYSTEM;
size_t PMemoryManager::s_szApplicationMemory = SIZE_MEMORY_APPLICATION;
size_t PMemoryManager::s_szPage = SIZE_PAGE;
size_t PMemoryManager::s_szSlotUnit = SIZE_SLOT_UNIT;

char* PMemoryManager::s_pSystemMemeory = nullptr;
PMemoryStatic* PMemoryManager::s_pMemoryStatic = nullptr;;
char* PMemoryManager::s_pApplicationMemeory = nullptr;;
PMemoryDynamic* PMemoryManager::s_pMemoryDynamic = nullptr;;

void PMemoryManager::Allocate() {
    try {
        MLOG_HEADER("PMemoryManager::Allocate");
        // system memory allocation
        s_pSystemMemeory = new char[s_szSystemMemory];
        s_pMemoryStatic = new(s_pSystemMemeory, s_szSystemMemory, "s_pMemoryStatic") PMemoryStatic();
        s_pMemoryStatic->Initialize();
        MLOG_SHOW("PMemoryManager::Allocate", s_pMemoryStatic);

        // aplication memorty allocation
        s_pApplicationMemeory = new char[s_szApplicationMemory];
        MemoryDynamic::s_pPageList = new("pPageList") PageList((size_t)s_pApplicationMemeory, s_szApplicationMemory, s_szPage);
        s_pMemoryDynamic = new("s_pMemoryDynamic") PMemoryDynamic(s_szSlotUnit);
        s_pMemoryDynamic->Initialize();
        MLOG_FOOTER("PMemoryManager::Allocate");
    } catch (Exception& exception) {
        exception.Println();
    }
}

void PMemoryManager::Delocate() {
    try {
        MLOG_HEADER("PMemoryManager::Delocate");
        s_pMemoryDynamic->Finalize();
        delete s_pMemoryDynamic;
        delete[] s_pApplicationMemeory;

        s_pMemoryStatic->Finalize();
    //   MLOG_SHOW(s_pMemoryDynamic);
        delete s_pMemoryStatic;
        delete[] s_pSystemMemeory;
        MLOG_FOOTER("PMemoryManager::Delocate");
    } catch (Exception& exception) {

    }
}