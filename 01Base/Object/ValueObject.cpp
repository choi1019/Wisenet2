#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Exception.h>
#include <01Base/Aspect/Log.h>

IMemory* ValueObject::s_pMemory = nullptr;

void* ValueObject::operator new (size_t szObject, const char* sMessage) {
    void* pObject = ValueObject::s_pMemory->Malloc(szObject, sMessage);
    return pObject;
}
void* ValueObject::operator new[] (size_t szThis, const char* sMessage) {
    return ValueObject::operator new(szThis, sMessage);
}
void ValueObject::operator delete(void* pObject) {
    ValueObject::s_pMemory->Free(pObject);
}
void ValueObject::operator delete[](void* pObject) {
    ValueObject::s_pMemory->Free(pObject);
}
// dummy
void ValueObject::operator delete(void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "ValueObject::delete[]", (size_t)pObject);
}
void ValueObject::operator delete[](void* pObject, const char* sMessage) {
     throw Exception((unsigned)IMemory::EException::_eNotSupport, "ValueObject::delete[]", (size_t)pObject);
}
