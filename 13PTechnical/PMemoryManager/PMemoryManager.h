#pragma once

#include <13PTechnical/typedef.h>
#define _PMemoryManager_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryManager)
#define _PMemoryManager_Name "PMemoryManager"

#include <13PTechnical/PMemoryManager/PMemoryStatic.h>
#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

class PMemoryManager {
public:
    static size_t s_szSystemMemory;
    static char* s_pSystemMemeoryAllocated;
    static PMemoryStatic* s_pMemoryStatic;

    static size_t s_szApplicationMemory;
    static size_t s_szPage;
    static size_t s_szSlotUnit;
    static char* s_pApplicationMemeoryAllocated;
    static PMemoryDynamic* s_pMemoryDynamic;

	static void Allocate();
	static void Delocate();
};