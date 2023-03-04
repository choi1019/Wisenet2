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
	MapPair* next;

	MapPair() {
		first = KEYTYPE();
		second = VALUETYPE();
		next = nullptr;
	}
	MapPair(KEYTYPE key, VALUETYPE value) : first(key), second(value), next(nullptr) {}
	inline bool operator==(const MapPair& rhs) { return (first == rhs.first && second == rhs.second); }
	inline bool operator!=(const MapPair& rhs) { return !(first == rhs.first && second == rhs.second); }
	inline void operator=(const MapPair& rhs) { first = rhs.first; second = rhs.second; }
};

template <class KEYTYPE, class VALUETYPE>
class MapIterator : public ValueObject {
private:
	MapPair<KEYTYPE, VALUETYPE>* pElement;
public:
	MapIterator(MapPair<KEYTYPE, VALUETYPE>* pElement) : pElement(pElement) {}
	MapIterator() : pElement(nullptr) {}

	inline MapIterator operator++() {
		pElement = pElement->next;
		return pElement;
	}
	inline MapIterator operator++(int) {
		MapIterator current = pElement;
		pElement = pElement->next;
		return current;
	}
	// inline MapIterator operator+(const int i) {	return pElement + i; }
	// inline MapIterator operator-(const int i) { return pElement - i; }
	inline MapPair<KEYTYPE, VALUETYPE>& operator*() { return *pElement; }
	inline MapPair<KEYTYPE, VALUETYPE>* operator->() { return pElement; }
	inline bool operator==(const MapIterator& rhs) { return pElement == rhs.pElement; }
	inline bool operator!=(const MapIterator& rhs) { return pElement != rhs.pElement; }
	inline void operator=(const MapIterator& rhs) { return pElement = rhs.pElement; }
};

template <class KEYTYPE, class VALUETYPE, int MAXLENGTH = 10>
class Map : public Collection {
private:
	int length;
	MapPair<KEYTYPE, VALUETYPE> *m_pHead;

public:
	typedef MapIterator<KEYTYPE, VALUETYPE> Iterator;
	typedef MapPair<KEYTYPE, VALUETYPE> Pair;

	Map(int nClassId = _Map_Id, const char* pcClassName = _Map_Name)
		: Collection(nClassId, pcClassName)
		, m_pHead(nullptr)
		, length(0)
	{
	}
	virtual ~Map() {
		for (int i=0;i<length; i++) {
			Pair* pDelete = m_pHead;
			m_pHead = m_pHead->next;
			delete pDelete;
		}
	}

	virtual void Initialize() { Collection::Initialize(); }
	virtual void Finalize() { Collection::Finalize(); }

	inline Iterator begin() { return Iterator(m_pHead); }
	inline Iterator end() {	return nullptr;	}

	inline int Size() const { return this->length; }
	inline void SetSize(int length) { this->length = length; }
	inline int Maxsize() const { return MAXLENGTH; }
	inline bool Empty() { return (length == 0) ? true : false; }
	inline void Clear() {
		for (int i=0;i<length; i++) {
			Pair* pDelete = m_pHead;
			m_pHead = m_pHead->next;
			delete pDelete;
		}
		this->length = 0;
	}
	inline Iterator First() { return Iterator(m_pHead); }

	inline Iterator Find(KEYTYPE key)
	{
		Iterator iterator = begin();
		for (; iterator != end(); ++iterator) {
			if (iterator->first == key) {
				return iterator;
			}
		}
		return iterator;
	}

	inline bool Remove(KEYTYPE key) {
		Pair* previous = nullptr;
		Pair* current = m_pHead;
		while (current != nullptr) {
			if (current->first == key) {
				break;
			}
			previous = current;
			current = current->next;
		}
		// not found
		if (current == nullptr) {
			return false;
		}
		// found, remove
		else {
			if (current == m_pHead) {
				m_pHead = current->next;
			}
			else {
				previous->next = current->next;
			}
			delete current;
			this->length--;
			return true;
		}
	}

	inline bool Add(Pair mapPair) {
		// if found, do not add
		Iterator iterator = this->Find(mapPair.first);
		if (iterator != end()) {
			return false;
		}
		// not found, add
		Pair *pNewPair = new("Pair") Pair(mapPair);
		pNewPair->next = m_pHead;	
		m_pHead = pNewPair;

		this->length++;
		return true;
	}

	inline bool Add(const KEYTYPE key, VALUETYPE element)
	{
		Pair mapPair(key, element);
		return Add(mapPair);
	}

	VALUETYPE& operator[](const KEYTYPE& key)
	{
		// if found, return
		Iterator iterator = this->Find(key);
		if (iterator != end()) {
			return iterator->second;
		}

		// not found, add
		Pair *pNewPair = new("Pair") Pair();
		pNewPair->next = m_pHead;	
		m_pHead = pNewPair;

		m_pHead->first = key;
		this->length++;
		return m_pHead->second;
	}
};