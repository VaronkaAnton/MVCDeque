// Deque class as Model for MVC pattern

#pragma once
#undef UNICODE
#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <initializer_list>
#include <utility>
#include <exception>
#include <string>
#include <math.h>
#include <strstream>
using namespace std;


struct position {
	int chunkNum;
	int elementNum;
}; //a structure describing a position of element in a deque

template <class T>
class KDeque {
protected:
	int count; //number of elements in deque
	T** chunkmap; //a pointer to the first element of array of pointers to the chunks; also chunkmap.begin()
	int chunkmapSize; //number of chunks available
	int firstChunk = 1; //index of the first occupied position in chunkmap
	int firstElement = 1; //index of the first occupied position in the first chunk
	const int numElementsInChunk = 4;




	void expandMap() {
		// double the size of the block map, copying the
		// current set of pointers into the middle of the 
		// new map array
		T** newMap = new T*[2 * chunkmapSize];
		fill_n(newMap, 2 * chunkmapSize, (T*)0);
		for (int i = 0; i < 2 * chunkmapSize; i++)
			newMap[i] = new T[numElementsInChunk];
		copy(chunkmap + firstChunk, chunkmap + chunkmapSize, newMap + chunkmapSize / 2);
		//copies pointers to the chunks

		delete[] chunkmap;

		chunkmap = newMap; //both are pointers to first elements of arrays
		firstChunk = chunkmapSize / 2;
		chunkmapSize *= 2;
	}

public:

	position indexAt(int n) const {
		position pos;
		pos.chunkNum = firstChunk;
		if (n < numElementsInChunk - firstElement)
			pos.elementNum = firstElement + n;
		else
		{
			n -= numElementsInChunk - firstElement;
			pos.chunkNum++;
			int k = n / numElementsInChunk;
			int t = n % numElementsInChunk;
			pos.chunkNum += k;
			pos.elementNum = t;
		}
		return pos;
	};
	class iterator {
	private:
		T* p;
		int chNum;
		int elNum;
		KDeque<T>* d;
	public:
		iterator(T* p, KDeque<T>* d) : p(p), d(d) {
			chNum = d->firstChunk;
			elNum = d->firstElement;
		};

		bool operator!=(iterator const& other) const { return p != other.p; }
		bool operator==(iterator const& other) const { return p == other.p; };
		iterator operator++() {
			if (elNum == d->numElementsInChunk - 1)
			{
				chNum++;
				elNum = 0;
			}
			else
			{
				elNum++;
			}
			p = &(d->chunkmap[chNum][elNum]);
			return *this;
		}
		T& operator* () { return *p; }
	};

	iterator begin() {
		iterator it(&(front()), this);
		return it;
	}
	iterator end() {
		iterator it(&(back()) + 1, this);
		return it;
	}

	KDeque()
	{
		chunkmapSize = 4;
		count = 0;
		chunkmap = new T*[chunkmapSize];
		for (int i = 0; i < chunkmapSize; i++)
			chunkmap[i] = new T[numElementsInChunk];
	}

	KDeque(const KDeque &c) : //copy constructor
		chunkmapSize(c.chunkmapSize), firstChunk(c.firstChunk),
		firstElement(c.firstElement), count(c.count)
	{
		chunkmap = new T*[chunkmapSize];
		for (int i = 0; i < chunkmapSize; i++)
			chunkmap[i] = new T[numElementsInChunk];
		for (int i = 0; i < chunkmapSize; i++)
			copy(c.chunkmap[i], c.chunkmap[i] + numElementsInChunk, chunkmap[i]);
	}

	KDeque& operator=(const KDeque& c) //copy assignment
	{
		if (this != &c)
		{
			count = c.count;
			chunkmapSize = c.chunkmapSize;
			firstChunk = c.firstChunk;
			firstElement = c.firstElement;
			chunkmap = new T*[chunkmapSize];
			for (int i = 0; i < chunkmapSize; i++)
				chunkmap[i] = new T[numElementsInChunk];
			for (int i = 0; i < chunkmapSize; i++)
				copy(c.chunkmap[i], c.chunkmap[i] + numElementsInChunk, chunkmap[i]);
		}
		return *this;
	}

	KDeque(KDeque &&m) : //move constructor
		chunkmap(nullptr), chunkmapSize(0), firstChunk(0),
		firstElement(0), count(0) {
		count = m.count;
		chunkmap = m.chunkmap;
		chunkmapSize = m.chunkmapSize;
		firstChunk = m.firstChunk;
		firstElement = m.firstElement;

		m.count = 0;
		m.chunkmap = nullptr;
		m.chunkmapSize = 0;
		m.firstChunk = 0;
		m.firstElement = 0;
	}

	KDeque operator=(KDeque&& m) //move assignment
	{
		if (this != &m)
		{
			delete[] chunkmap;

			count = m.count;
			chunkmap = m.chunkmap;
			chunkmapSize = m.chunkmapSize;
			firstChunk = m.firstChunk;
			firstElement = m.firstElement;

			m.count = 0;
			m.chunkmap = nullptr;
			m.chunkmapSize = 0;
			m.firstChunk = 0;
			m.firstElement = 0;
		}
		return *this;
	}

	bool operator == (KDeque& other)
	{
		if (count != other.count)
			return false;
		else
		{
			auto itThis = begin();
			auto itOther = other.begin();
			while (itThis != end())
			{
				if (!(*itThis == *itOther))
					return false;
				++itThis; ++itOther;
			}
		}
		return true;
	}

	bool operator != (KDeque& other)
	{
		if (count != other.count)
			return true;
		else
		{
			auto itThis = begin();
			auto itOther = other.begin();
			while (itThis != end())
			{
				if (*itThis != *itOther)
					return true;
				++itThis; ++itOther;
			}
		}
		return false;
	}

	KDeque& operator+=(KDeque& rhs) {
		auto it = rhs.begin();
		while (it != rhs.end())
		{
			push_back(*it);
			++it;
		}
		return *this;
	}

	KDeque operator+(KDeque& rhs) {
		KDeque sum;
		auto it = begin();
		while (it != end())
		{
			sum.push_back(*it);
			++it;
		}
		it = rhs.begin();
		while (it != rhs.end())
		{
			sum.push_back(*it);
			++it;
		}
		return sum;
	}

	KDeque(initializer_list<T> lst) {
		chunkmapSize = 4;
		count = 0;
		chunkmap = new T*[chunkmapSize];
		for (int i = 0; i < chunkmapSize; i++)
			chunkmap[i] = new T[numElementsInChunk];
		for (auto it = lst.begin(); it != lst.end(); it++)
			(*this).push_back(*it);
	}

	T operator[] (int n) {
		if (n < 0 || n >= count) {
			//throw 1;
		}
		else
		{
			position pos = indexAt(n);
			return chunkmap[pos.chunkNum][pos.elementNum];
		}
	}

	bool isEmpty() { return count ? 0 : 1; }

	T& front() {
		return chunkmap[firstChunk][firstElement];
	}

	T& back() {
		position pos = indexAt(count - 1);
		return chunkmap[pos.chunkNum][pos.elementNum];
	}

	const position& fst() const {
		position pos;
		pos.chunkNum = firstChunk;
		pos.elementNum = firstElement;
		return pos;
	}

	int& size() { return count; }

	void push_back(T n) {
		position pos = indexAt(count);
		if (pos.chunkNum == chunkmapSize)
		{
			expandMap();
			pos = indexAt(count);
		}
		chunkmap[pos.chunkNum][pos.elementNum] = n;
		count++;
	}

	void pop_back() { count--; }

	void push_front(T n) {
		position pos = indexAt(0); //first element position
		if (pos.elementNum > 0)
			pos.elementNum--;
		else
		{
			pos.chunkNum--;
			if (pos.chunkNum < 0)
			{
				expandMap();
				pos = indexAt(0); //new indices
				pos.chunkNum--;
			}
			pos.elementNum = numElementsInChunk - 1;
		}

		chunkmap[pos.chunkNum][pos.elementNum] = n;
		count++;
		//the begin of deque has also changed
		firstChunk = pos.chunkNum;
		firstElement = pos.elementNum;
	}

	void pop_front() {
		position pos = indexAt(0);
		if (pos.elementNum != numElementsInChunk - 1)
		{
			firstChunk = pos.chunkNum;
			firstElement = pos.elementNum + 1;
		}
		else
		{
			firstChunk = pos.chunkNum + 1;
			firstElement = 0;
		}
		count--;
	}

	void swap(KDeque& b)
	{
		KDeque c = move(*this);
		*this = move(b);
		b = move(c);
	}


	void fromString(char* str)
	{
		strstream in;
		in << str;
		T temp;
		while (in >> temp)
			(*this).push_back(temp);
	};

	string toString()
	{
		string str;
		for (int i = 0; i < count; i++)
		{
			str += to_string(chunkmap[indexAt(i).chunkNum][indexAt(i).elementNum]);
			str += ' ';
		}
		return str;
	}
	void clear()
	{
		if (!count)
		{
			delete[] chunkmap;
			chunkmap = nullptr;
		}
		count = 0;
	}
	~KDeque()
	{
		clear();
		//	cout << "THIS IS THE END. MY ONLY FRIEND THE END" << endl;
	}
};