#pragma once

#include "Bindable.h"

#include <vector>

/* interface class
* which only made of pure virtual functions
*/

//typedef std::vector<Bindable*> CBV;

class BV:
	public std::vector<Bindable*>
{
private:
	UINT find(BINDABLE_TYPE type, UINT start, UINT end) {
		if (end == start + 1) {
			return start;
		}
		int v = type - (*this)[(start + end) / 2]->getType();
		if (v < 0) {
			return find(type,start, (start + end) / 2);
		}
		return find(type, (start + end) / 2,end);
	}

public:
	BV() :
		std::vector<Bindable*>()
	{
	}

	~BV()
	{
		printf("delete BV\n");
	}

	Bindable* find(BINDABLE_TYPE type) {
		return (*this)[find(type, 0, size())];
	}

	void insert(Bindable* item){
		
		if (size() == 0) {
			push_back(item);
			return;
		}
		UINT i = size();
		push_back(0);

		while ((i>0) && (item->getType()< (*this)[i-1]->getType()))
		{
			printf("%d->%d\n", i - 1, i);
			(*this)[i] = (*this)[i - 1];
			i--;
		}
		(*this)[i] = item;
		
	}

	void print() {
		for (auto a: (*this))
		{
			printf("(%d, %X) ", a->getType(), a);
		}
	}

	void bind( Graphics &gfx) {
		for (auto a : (*this))
		{
			a->bind(gfx);
		}
	}

	void Release() {
		for (auto a : (*this))
		{
			a->release();
		}
	}

};

class Drawable
{
private:
	BV cbv; // class bindable vector
	unsigned int numIndeceies;

public:
	BV& getBindables();
	Bindable* operator[](BINDABLE_TYPE i);
	unsigned int getIndecesNumber();
	void AddBindable(Bindable* bindable);
	void Draw(Graphics& gfx);

public:
	Drawable();
	virtual ~Drawable() {
		
		std::cout << "delete Drawable at :" << this << '\n';

		for (auto a : cbv)
		{
			a->release();
		}
	};
};



