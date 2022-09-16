#pragma once

#include "Bindable.h"
#include <vector>
#include "IndexBuffer.h"

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

	unsigned int getIndecesNumber();

	void AddBindable(Bindable* bindable);

	~GCLASS();
};



class Drawable
{
private:
	GCLASS* pGCLASS;

public:
	
	void setGCLASS(GCLASS& gclass );

	void setGCLASS(GCLASS* gclass);

	GCLASS& getGCLASS();

	void setGCLASS(const char* className);

	void Draw(Graphics& gfx);

public:
	//virtual void updateAPI() = 0;
	virtual DirectX::XMMATRIX GetTransform() = 0;

public:
	Drawable(std::string& className);
	Drawable(GCLASS* pGCLASS);
};



