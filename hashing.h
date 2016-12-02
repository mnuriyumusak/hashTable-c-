#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <array>
#ifndef HASHING_H_INCLUDED
#define HASHING_H_INCLUDED


template <class HashedObj>
class hashing
{
public:
	explicit hashing(int size = 101) : array(nextPrime(size))
	{ 
		size = nextPrime(size);
		makeEmpty(); 
		currentTotalItem = 0;
		insertTableS = 0;
		deleteTableS = 0;
		findTableS = 0;
		insertTableF = 0;
		deleteTableF = 0;
		findTableF = 0;
		hashTableSize = size;
		howManyProbesConducted = 0;
		transactionStatus = false;
	}

	int nextPrime(int n);
	bool isPrime(int n);
	void makeEmpty();
	void insert(const HashedObj & x);
	void remove(const HashedObj & x);
	enum EntryType { ACTIVE, EMPTY, DELETED };
	//below should be private
	int findPos(const HashedObj & x) const;
	int find(const HashedObj & x);
	int currentTotalItem;
	int insertTableS;
	int deleteTableS;
	int findTableS;
	int insertTableF;
	int deleteTableF;
	int findTableF;
	int hashTableSize;
	int howManyProbesConducted;
	bool transactionStatus;
	int getTotalTransaction();
private:
	struct HashEntry
	{
		HashedObj element;
		EntryType   info;

		HashEntry(const HashedObj & e = HashedObj(), EntryType i = EMPTY) 
		{ 
			element=e ;
			info= i ;
		}

		HashEntry(HashedObj && e, EntryType i = EMPTY)
		{
			element=std::move(e);
			info= i ;
		}
	};

	std::vector<HashEntry> array;
	bool isActive(int currentPos) const;
	size_t myHash(const HashedObj & x) const;
	
};

template <class HashedObj>
int hashing<HashedObj>::getTotalTransaction()
{
	return insertTableS+deleteTableS+findTableS+insertTableF+deleteTableF+findTableF+hashTableSize;
}


template <class HashedObj>
int hashing<HashedObj>::nextPrime(int n)
{
	if (n % 2 == 0)
		n++;

	for (; !isPrime(n); n += 2)
		;

	return n;
}

template <class HashedObj>
bool hashing<HashedObj>::isPrime(int n)
{
	if (n == 2 || n == 3)
		return true;

	if (n == 1 || n % 2 == 0)
		return false;

	for (int i = 3; i * i <= n; i += 2)
		if (n % i == 0)
			return false;

	return true;
}

template <class HashedObj>
void hashing<HashedObj>::makeEmpty()
{
	for (auto & entry : array)
		entry.info = EMPTY;
}

template <class HashedObj>
size_t hashing<HashedObj>::myHash(const HashedObj & x) const
{
	return x % array.size();
}

template <class HashedObj>
int hashing<HashedObj>::findPos(const HashedObj & x) const
{
	int currentPos = myHash(x);

	while (array[currentPos].info == ACTIVE && array[currentPos].element != x)      /* search for item */
	{
		currentPos += 1;  // Compute linear probe
		if (currentPos >= array.size())
			currentPos -= array.size();
	}

	return currentPos;

}

template <class HashedObj>
int hashing<HashedObj>::find(const HashedObj & x) 
{
	int currentPos = myHash(x);
	int finish = 0;

	while (array[currentPos].info == ACTIVE && array[currentPos].element != x && finish != 2)      /* search for item */
	{
		currentPos += 1;  // Compute linear probe
		if (currentPos >= array.size())
		{
			currentPos -= array.size();
			finish++;
		}
	}
	if (array[currentPos].element == x && isActive(currentPos))
	{
		findTableS++;
		transactionStatus = true;
		howManyProbesConducted = currentPos - myHash(x);
		return currentPos;
	}
	else
	{
		findTableF++;
		transactionStatus = false;
		return -1;
	}
}

template <class HashedObj>
void hashing<HashedObj>::insert(const HashedObj & x)
{
	// Insert x as active
	int currentPos = findPos(x);
	if (isActive(currentPos))
	{
		insertTableF++;
		transactionStatus = false;
		return;			// return without inserting
	}
	
	array[currentPos] = HashEntry(x, ACTIVE); // create an active hash entry
	currentTotalItem++;
	insertTableS++;
	transactionStatus = true;
	howManyProbesConducted = currentPos -myHash(x);
}

/**
* Remove item x from the hash table.
*/
template <class HashedObj>
void hashing<HashedObj>::remove(const HashedObj & x)
{
	int currentPos = findPos(x);
	if (array[currentPos].element == x)
	{
		if (isActive(currentPos))  // item to be deleted found
		{
			array[currentPos].info = DELETED;
			deleteTableS++;
			currentTotalItem--;
			transactionStatus = true;
			howManyProbesConducted = currentPos - myHash(x);
		}
		else
		{
			deleteTableF++;
			transactionStatus = false;
		}
			
	}
	else
	{
		deleteTableF++;
		transactionStatus = false;
	}
		
}

template <class HashedObj>
bool hashing<HashedObj>::isActive(int currentPos) const
{
	return array[currentPos].info == ACTIVE;
}
















#endif 