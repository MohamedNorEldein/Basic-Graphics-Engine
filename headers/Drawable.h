#pragma once

#include "Bindable.h"
#include <vector>

/* interface class
* which only made of pure virtual functions
*/

typedef std::vector<Bindable*> CBV;

class GCLASS{
private:
	char CLASS_NAME[50] = { 0 };
	CBV cbv; // class bindable vector
	unsigned int numIndeceies;

public:
	GCLASS(const char* className);

	const char* getClassName();

	CBV& getBindables();

	Bindable* operator[](bindableType i) {
		for (auto a: cbv) {
			if (i == a->getType())
				return a;
		}
		return nullptr;
	}

	unsigned int getIndecesNumber();

	void AddBindable(Bindable* bindable);

	~GCLASS();
};

AugmantedTree::Map<const char*, GCLASS*>& GetGMAP();

class Drawable
{
private:
	GCLASS* pGCLASS;

protected:
	virtual	void _setGCLASS(GCLASS*);

	void setGCLASS(GCLASS& gclass);

	void setGCLASS(GCLASS* gclass);

	void setGCLASS(const char* className);

	GCLASS& getGCLASS();

public:

	void Draw(Graphics& gfx);


public:
	Drawable(const char* className);
	Drawable(const GCLASS& pGCLASS);
	Drawable();


	virtual ~Drawable() {
		std::cout << "delete Drawable at :" << this << '\n';

	};
};



