#include "Drawable.h"
#include "IndexBuffer.h"

AugmantedTree::Map<const char*, GCLASS*> Gmap;

AugmantedTree::Map<const char*, GCLASS*>& GetGMAP()
{
	return Gmap;
}

GCLASS::GCLASS(const char* className) :
	numIndeceies(0)
{
	strcpy(CLASS_NAME, className);


	Gmap.insert(CLASS_NAME, this);

}

const char* GCLASS::getClassName() {
	return CLASS_NAME;
}

CBV& GCLASS::getBindables() {
	return cbv;
}

unsigned int GCLASS::getIndecesNumber() {
	return numIndeceies;
}

void GCLASS::AddBindable(Bindable* bindable)
{

	cbv.push_back(bindable);

	if (bindable->getType() == _Indexbuffer) {
		numIndeceies = ((IndexBuffer*)bindable)->getIndecesNumber();
	}

}

GCLASS::~GCLASS()
{
	std::cout << "delete GCLASS at :" << this << '\n';
	Gmap.remove(CLASS_NAME);
}


void Drawable::setGCLASS(GCLASS& gclass)
{
	_setGCLASS(&gclass);
}



void Drawable::_setGCLASS(GCLASS* gclass) {
	pGCLASS = gclass;
}


void Drawable::setGCLASS(GCLASS* gclass)
{
	_setGCLASS(gclass);
}


GCLASS& Drawable::getGCLASS()
{
	return *pGCLASS;
}

void Drawable::setGCLASS(const char* className)
{
	_setGCLASS(Gmap[className]);
}

void Drawable::Draw(Graphics& gfx) {
	for (auto b : pGCLASS->getBindables()) {
		b->bind(gfx);
	}

	gfx.getcontext()->DrawIndexed(pGCLASS->getIndecesNumber(), 0, 0);

}

Drawable::Drawable(const char* className) :
	pGCLASS(Gmap[className])
{
}

Drawable::Drawable(const GCLASS& pGCLASS) :
	pGCLASS((GCLASS*)&pGCLASS)
{
}

Drawable::Drawable() :
	pGCLASS(nullptr)
{

}


