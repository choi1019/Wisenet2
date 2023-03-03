#pragma once

#include <01Base/typedef.h>
#define _Map_Id _GET_CLASS_UID(_ELayer_Base::_eMap)
#define _Map_Name "Map" 

#include "Collection.h"

template <class KEYTYPE, class VALUETYPE>
class MapPair : public ValueObject {
public:
	KEYTYPE first;
	VALUETYPE second;
	MapPair *pNext;
	
	MapPair() { 
		first = KEYTYPE (); 
		second = VALUETYPE();
		pNext = nullptr;
	}
	MapPair(KEYTYPE key, VALUETYPE value) : first(key), second(value) {}
	inline bool operator==(const MapPair& rhs) { return (first == rhs.first && second == rhs.second); }
	inline bool operator!=(const MapPair& rhs) { return !(first == rhs.first && second == rhs.second); }
};

template <class KEYTYPE, class VALUETYPE>
class MapIterator : public ValueObject {
private:
	MapPair<KEYTYPE, VALUETYPE>* pElement;
public:
	MapIterator(MapPair<KEYTYPE, VALUETYPE>* pElement) : pElement(pElement) {}
	MapIterator() : pElement(nullptr) {}

	inline MapIterator operator++() {
		pElement = pElement->pNext;
		return pElement;
	}
	inline MapIterator operator++(int) {
		MapIterator current = pElement; 
		pElement = pElement->pNext;
		return current;
	}
	// inline MapIterator operator+(const int i) {	return pElement + i; }
	// inline MapIterator operator-(const int i) { return pElement - i; }
	inline MapPair<KEYTYPE, VALUETYPE>& operator*() { return *pElement; }
	inline MapPair<KEYTYPE, VALUETYPE>* operator->() { return pElement; }
	inline bool operator==(const MapIterator& rhs) { return pElement == rhs.pElement; }
	inline bool operator!=(const MapIterator& rhs) { return pElement != rhs.pElement; }
	inline bool operator=(const MapIterator& rhs) { return pElement = rhs.pElement; }
};

template <class KEYTYPE, class VALUETYPE, int MAXLENGTH = 100>
class Map: public Collection {
private:
	int length;
	MapPair<KEYTYPE, VALUETYPE> *m_pFront, *m_pRear, m_pRearMax;

public:
	typedef MapIterator<KEYTYPE, VALUETYPE> Iterator;
	typedef MapPair<KEYTYPE, VALUETYPE> Pair;

	Map(int nClassId = _Map_Id, const char *pcClassName = _Map_Name) 
		: Collection(nClassId, pcClassName)
		, m_pFront(nullptr)
		, m_pRear(nullptr)
		, length(0) 
		{
		}
	virtual ~Map() {
		while (m_pFront != nullptr) {
			Pair *pDelete = m_pFront;
			m_pFront = m_pFront->pNext;
			delete pDelete;
		}
	}

	virtual void Initialize() {	Collection::Initialize(); }
	virtual void Finalize() { Collection::Finalize(); }

	inline Iterator begin() { return Iterator(m_pFront); }
	inline Iterator end() { return Iterator(m_pRear->pNext); }
	inline Iterator rear() { return Iterator(m_pRear); }

	inline int Size() const { return this->length; }
	inline int Maxsize() const { return MAXLENGTH; }
	inline bool Empty() { return (length==0)? true: false; }
	inline void Clear() { 
		while (m_pFront != nullptr) {
			Pair *pDelete = m_pFront;
			m_pFront = m_pFront->pNext;
			delete pDelete;
		}
		this->length = 0; 
	}
	inline Iterator First() { return Iterator(m_pFront); }

	inline Iterator Find(KEYTYPE key) 
	{
		Iterator itr = begin();
		for ( ; itr != end(); ++itr) {
			if (itr->first == key) {
				return itr;
			}
		}
		return itr;
	}

	inline bool Remove(KEYTYPE key) {
		Iterator previous = begin();
		Iterator current = previous;
		while (current != end()) {
			if (current->first == key) {
				break;
			}
			previous = current;
			++current;
		}
		// not found
		if (pCurrent == end()) {
			return false;
		} 
		// found, remove
		else {
			if (previous == current) {
				m_pFront = m_pFront->pNext;
			} else {
				previous->pNext = current->pNext;
			}
			delete current.pElement;
			this->length--;
			return true;
		}
	}

	inline bool Add(MapPair<KEYTYPE, VALUETYPE>  mapPair) {
		if (m_pFront == nullptr) {
			m_pFront = new Pair();
			m_pRear = m_pFront;
		} else {
			Iterator itr = this->Find(mapPair.first);
			// found, do not add
			if (itr != this->end()) {
				return false;
			}
			m_pRear->pNext = new Pair();
			m_pRear = m_pRear->pNext;
		}
		*m_pRear = mapPair;
		this->length++;
		return true;
	}

	inline bool Add(const KEYTYPE& key, VALUETYPE& element)
	{
		Pair mapPair(key, element);
		return Add(mapPair);
	}

	VALUETYPE& operator[](const KEYTYPE& key)
	{
		Iterator current = begin();
		while (current != end) {
			if (current->first == key) {
				return current->second;
			}
			++current;
		}
		m_pRear->pNext = new Pair();
		m_pRear = m_pRear->pNext;
		Iterator itrRear(m_pRear);
		itrRear->first = key;
		this->length++;
		return itrRear->second;
	}
};
