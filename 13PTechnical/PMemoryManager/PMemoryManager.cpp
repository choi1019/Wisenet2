#include <13PTechnical/PMemoryManager/PMemoryManager.h>

#include <13PTechnical/PMemoryManager/Config.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

size_t PMemoryManager::s_szSystemMemory = SIZE_MEMORY_SYSTEM;
size_t PMemoryManager::s_szApplicationMemory = SIZE_MEMORY_APPLICATION;
size_t PMemoryManager::s_szPage = SIZE_PAGE;
size_t PMemoryManager::s_szSlotUnit = SIZE_SLOT_UNIT;

char* PMemoryManager::s_pSystemMemeoryAllocated = nullptr;
PMemoryStatic* PMemoryManager::s_pMemoryStatic = nullptr;;
char* PMemoryManager::s_pApplicationMemeoryAllocated = nullptr;;
PMemoryDynamic* PMemoryManager::s_pMemoryDynamic = nullptr;;

void PMemoryManager::Allocate() {
    MLOG_HEADER("PMemoryManager::Allocate");
    // system memory allocation
    s_pSystemMemeoryAllocated = new char[s_szSystemMemory];
    s_pMemoryStatic = new(s_pSystemMemeoryAllocated, s_szSystemMemory, "s_pMemoryStatic") PMemoryStatic();
    s_pMemoryStatic->Initialize();
    MLOG_SHOW(s_pMemoryStatic);

    // aplication memorty allocation
    s_pApplicationMemeoryAllocated = new char[s_szApplicationMemory];
    s_pMemoryDynamic = new(s_pApplicationMemeoryAllocated, s_szApplicationMemory, "s_pMemoryDynamic") PMemoryDynamic(s_szPage, s_szSlotUnit);
    s_pMemoryDynamic->Initialize();
    MLOG_SHOW(s_pMemoryDynamic);
    MLOG_FOOTER("PMemoryManager::Allocate");
}

void PMemoryManager::Delocate() {
    MLOG_HEADER("PMemoryManager::Delocate");
    s_pMemoryDynamic->Finalize();
    MLOG_SHOW(s_pMemoryStatic);
    delete s_pMemoryDynamic;
    delete[] s_pApplicationMemeoryAllocated;

    s_pMemoryStatic->Finalize();
    MLOG_SHOW(s_pMemoryDynamic);
    delete s_pMemoryStatic;
    delete[] s_pSystemMemeoryAllocated;
    MLOG_FOOTER("PMemoryManager::Delocate");
}