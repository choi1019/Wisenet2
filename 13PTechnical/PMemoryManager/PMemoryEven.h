// #pragma once

// #include <13PTechnical/typedef.h>
// #define _PMemoryEven_Id _GET_CLASS_UID(_ELayer_PTechnical::_ePMemoryEven)
// #define _PMemoryEven_Name "PMemoryEven"

// #include <03Technical/MemoryManager/MemoryEven.h>
// #include <pthread.h>

// class PMemoryEven : public MemoryEven {
// private:
// //	CRITICAL_SECTION CriticalSection;
// 	pthread_mutex_t m_mutex;

// protected:
// 	void Lock();
// 	void UnLock();

// public:
// 	PMemoryEven(
// 		size_t szSlotUnit,
// 		unsigned nClassId = _PMemoryEven_Id,
// 		const char* pcClassName = _PMemoryEven_Name);
// 	~PMemoryEven() override;

// 	void Initialize() override;
// 	void Finalize() override;

// 	void* Malloc(size_t szObject, const char* sMessage) override;
// 	bool Free(void* pObject) override;
// };