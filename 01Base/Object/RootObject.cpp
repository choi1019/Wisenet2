#include <01Base/Object/RootObject.h>
#include <01Base/Aspect/Directory.h>

unsigned RootObject::s_uObjectCount = 0;

RootObject::RootObject(unsigned nClassId, const char* pcClassName)
    : m_uObjectId(RootObject::s_uObjectCount++)
    , m_nClassId(nClassId)
{
    strcpy(m_pcClassName, pcClassName);
    this->m_eState = EState::eCreated;
    //Directory::s_dirClasses[m_nClassId] = m_pcClassName;
}
RootObject::~RootObject() 
{
    this->m_eState = EState::eDeleted;
}
