#pragma once

#include <01Base/Object/ValueObject.h>
#include <01Base/Aspect/Log.h>
#include <01Base/StdLib/Vector.h>

#define NUM_ITERATION 2
class DomainObject11 : public ValueObject {
private:
	Vector<int>* m_pVector[NUM_ITERATION];  // 248 Byte

public:
	DomainObject11() 
		: ValueObject()
	{
		for (int i=0; i<NUM_ITERATION; i++) {
			m_pVector[i] = new("m_pVector") Vector<int>();
		}
	}
	~DomainObject11() {
		for (int i=0; i<NUM_ITERATION; i++) {
			delete m_pVector[i];
		}
	}

	void Run() {
		for (int j=0; j<NUM_ITERATION; j++) {
			for (int i = 0; i < this->m_pVector[j]->Max_size(); i++) {
				this->m_pVector[j]->Add(i);
				LOG(i, "-");
			}
		}
	}
};
