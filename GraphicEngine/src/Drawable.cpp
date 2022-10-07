#include "Drawable.h"
#include "IndexBuffer.h"



 /******************************************************************/
/*						Drawable								  */		


BV& Drawable::getBindables() {
	return cbv;
}

Bindable* Drawable::operator[](BINDABLE_TYPE type) {
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

	if (bindable->getType() == BINDABLE_TYPE::INDEX_BUFFER) {
		numIndeceies = ((IndexBuffer*)bindable)->getIndecesNumber();
	}
}


void Drawable::Draw(Graphics& gfx) {
	for (auto a : cbv)
	{
		a->bind(gfx);
	}
	gfx.getcontext()->DrawIndexed(numIndeceies, 0, 0);
}

Drawable::Drawable() :
	cbv()
{
}

