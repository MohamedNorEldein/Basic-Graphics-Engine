#include "Drawable.h"
#include "IndexBuffer.h"
#include "mstring.h"
#include "List.h"
#include "AugmantedTree.h"
#include "CBuffer.h"


AugmantedTree::Map<const char *, GCLASS*> Gmap;


GCLASS::GCLASS(const char* className) :
	numIndeceies(0)
{
	strcpy(CLASS_NAME, className);
	for (int i = 0; i < _unspecified; ++i) {
		cbv.push_back(0);
	}
	/*
	AugmantedTree::strT<GCLASS*> st;
	strcpy(st.key, className);
	st.data = this;
	*/
	Gmap.insert(className, this);

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
	if (bindable->getType() < _unspecified) {
		cbv[bindable->getType()] = bindable;
	}
	else {
		cbv.push_back(bindable);
	}
	if (bindable->getType() == _Indexbuffer) {
		numIndeceies = ((IndexBuffer*)bindable)->getIndecesNumber();
	}

}

GCLASS::~GCLASS()
{
	Gmap.remove(CLASS_NAME);
}




void Drawable::setGCLASS(GCLASS& gclass)
{
	pGCLASS = &gclass;
}


void Drawable::setGCLASS(GCLASS* gclass)
{
	pGCLASS = gclass;
}


GCLASS& Drawable::getGCLASS()
{
	return *pGCLASS;
}

void Drawable::setGCLASS(const char* className)
{
	pGCLASS = Gmap[className];
}

void Drawable::Draw(Graphics& gfx) {

	//updateAPI();
	((TransformCBuffer*)pGCLASS->getBindables()[_TransforCBuffer])->setDrawableParent(this);

	for (auto b : pGCLASS->getBindables()) {
		b->bind(gfx);
	}

	pGCLASS->getBindables()[0]->GetContext(gfx)->DrawIndexed(pGCLASS->getIndecesNumber(), 0, 0);

}

Drawable::Drawable(std::string& className):
	pGCLASS(Gmap[className.c_str()])
{
}

Drawable::Drawable(GCLASS* pGCLASS) :
	pGCLASS(pGCLASS)
{

}



