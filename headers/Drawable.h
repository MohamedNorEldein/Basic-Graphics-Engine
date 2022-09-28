#pragma once

#include "Bindable.h"

#include <vector>

/* interface class
* which only made of pure virtual functions
*/

//typedef std::vector<Bindable*> CBV;

class BV:
	public Array<Bindable*,16>
{
private:
	UINT find(bindableType type, short start, short end) {
		if (end == start) {
			return start;
		}
		UINT v = type - data[(start + end) / 2]->getType();

		if (v > 0) {
			return find(type, (start + end) / 2 + 1, end);
		}
		if (v < 0) {
			return find(type, start, (start + end) / 2);
		}
		return ((start + end) / 2);
	}

public:
	BV() :
		Array()
	{
	}

	Bindable* find(bindableType type) {
		return data[find(type, 0, this->len - 1)];
	}

	void insert(Bindable* item){
		UINT pos = find(item->getType(), 0u, len);
		memcpy(data + pos, pos + data + 1, sizeof(void*) * len);
		data[pos] = item;
		len++;
	}

};

class Drawable
{
private:
	BV cbv; // class bindable vector
	unsigned int numIndeceies;

public:
	BV& getBindables();
	Bindable* operator[](bindableType i);
	unsigned int getIndecesNumber();
	void AddBindable(Bindable* bindable);
	void Draw(Graphics& gfx);

public:
	Drawable();
	virtual ~Drawable() {
		
		std::cout << "delete Drawable at :" << this << '\n';

	};
};



