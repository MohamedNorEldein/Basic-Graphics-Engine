#include "Drawable.h"
#include "IndexBuffer.h"



 /******************************************************************/
/*						Drawable								  */		


BV& Drawable::getBindables() {
	return cbv;
}

Bindable* Drawable::operator[](bindableType type) {
	auto b= cbv.find(type);
	if (b->getType() == type) {
		return b;
	}
	throw NotFound();
}

unsigned int Drawable::getIndecesNumber() {
	return numIndeceies;
}

void Drawable::AddBindable(Bindable* bindable) {

	cbv.insert(bindable);

	if (bindable->getType() == _Indexbuffer) {
		numIndeceies = ((IndexBuffer*)bindable)->getIndecesNumber();
	}
}


void Drawable::Draw(Graphics& gfx) {
	for (int i = 0; i < cbv.length(); i++)
	{
		cbv[i]->bind(gfx);
	}
	gfx.getcontext()->DrawIndexed(numIndeceies, 0, 0);
}

Drawable::Drawable() :
	cbv()
{
}

