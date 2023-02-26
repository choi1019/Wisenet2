
#include <92TestApplication/PTS1/PTC11/PTC11.h>
#include <92TestApplication/PTS1/PTC11/Config.h>

PTC11::PTC11(unsigned nClassId, const char* pcClassName)
    : TestCase(nClassId, pcClassName)
    , m_pDomainObject1(nullptr)
    , m_pDomainObject2(nullptr) 
{
}
PTC11::~PTC11() {
}

void PTC11::Initialize() {

}
void PTC11::Finalize() {
}
void PTC11::Run() {
    this->m_pDomainObject1 = new("PTC11::DomainObject1") DomainObject11();
    this->m_pDomainObject2 = new("PTC11::DomainObject2") DomainObject11();
    this->m_pDomainObject1->Run();
    this->m_pDomainObject2->Run();
        ValueObject::s_pMemory->Show("PTC11::DomainObject1/2");
    delete this->m_pDomainObject1;
    delete this->m_pDomainObject2;
        ValueObject::s_pMemory->Show("PTC11::DomainObject1/2");

    this->m_pDomainObject1 = new("PTC11::DomainObject1") DomainObject11();
    this->m_pDomainObject2 = new("PTC11::DomainObject2") DomainObject11();
    this->m_pDomainObject1->Run();
    this->m_pDomainObject2->Run();
    delete this->m_pDomainObject1;
    delete this->m_pDomainObject2;
}