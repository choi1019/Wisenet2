#pragma once

#include <22TestApplication/typedef.h>
#define _PTS2_Id _GET_TESTLAYER_UID(_ELayer_TestApplication::_ePTS2)
#define _PTS2_Name "PTS2"
#include <22TestApplication/PTS2/Config.h>

#include <13PTechnical/PMemoryManager/PMemoryStatic.h>
#include <13PTechnical/PMemoryManager/PMemoryDynamic.h>

#include <21TestPlatform/TestCase/TestSuite.h>
#include <22TestApplication/PTS2/PTC21/PTC21.h>
#include <22TestApplication/PTS2/PTC22/PTC22.h>

template <int SIZE_SYSTEM_MEMORY, int SIZE_USER_MEMORY, int SIZE_PAGE, int SIZE_SLOT_UNIT>
class PTS2: public TestSuite {
private:
	size_t m_szSystemMemory;
	char* m_pSystemMemeoryAllocated;
	PMemoryStatic* m_pMemoryStatic;

	size_t m_szUserMemory;
	char* m_pUserMemeoryAllocated;
	PMemoryDynamic* m_pMemoryDynamic;

public:
	PTS2(
		unsigned classId = _PTS2_Id,
		const char* pClassName = _PTS2_Name)
		: TestSuite(classId, pClassName)
		, m_pMemoryStatic(nullptr)
		, m_pMemoryDynamic(nullptr)
		, m_pSystemMemeoryAllocated(nullptr)
		, m_pUserMemeoryAllocated(nullptr)
	{
	}
	virtual ~PTS2() {
	}

	void Initialize() {
		try {
			// system memory allocation
			m_szSystemMemory = SIZE_SYSTEM_MEMORY;
			m_pSystemMemeoryAllocated = new char[m_szSystemMemory];			
			m_pMemoryStatic = new(m_pSystemMemeoryAllocated, m_szSystemMemory) PMemoryStatic();
			m_pMemoryStatic->Initialize();
			m_pMemoryStatic->Show("m_pMemoryStatic::Initialize()");

			// aplication memorty allocation
			m_szUserMemory = SIZE_USER_MEMORY;
			m_pUserMemeoryAllocated = new char[m_szUserMemory];
			m_pMemoryDynamic = new(m_pUserMemeoryAllocated, m_szUserMemory) PMemoryDynamic(SIZE_PAGE, SIZE_SLOT_UNIT);
			m_pMemoryDynamic->Initialize();
			m_pMemoryDynamic->Show("m_pMemoryDynamic::Initialize()");

			this->add(new("PTC21") PTC21());
			this->add(new("PTC22") PTC22());

		} catch (Exception& exception) {
			exception.Println();
			exit(1);
		}
	}

	void Finalize() {
		try {
			// delete m_pMemory;
			m_pMemoryDynamic->Finalize();
			m_pMemoryDynamic->Show("");
			delete m_pMemoryDynamic;
			delete[] m_pUserMemeoryAllocated;

			m_pMemoryStatic->Finalize();
			m_pMemoryStatic->Show("");
			delete m_pMemoryStatic;
			delete[] m_pSystemMemeoryAllocated;
		}
		catch (Exception& exception) {
			exception.Println();
			exit(2);
		}
	}
};
