#pragma once
#include <01Base/Aspect/Aspect.h>
#include <01Base/StdLib/String.h>

class MemoryLog : public Aspect {
    private:
        String m_sName;
        bool m_bAllocated;
    public:

        MemoryLog(const char* sName, bool bAllocated = false) 
        : m_sName(sName)
        , m_bAllocated(bAllocated) { 
        }
        const char* GetName() { return this->m_sName.c_str(); }
        bool IsAllocated() { return this->m_bAllocated; }
        void SetBAllocated(bool bAllocated) { this->m_bAllocated = bAllocated; }
};