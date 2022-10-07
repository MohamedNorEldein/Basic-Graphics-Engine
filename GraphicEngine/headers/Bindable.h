
#include<Graphics.h>
#include <Vector>


#ifndef BINDABLE
#define BINDABLE

enum  BINDABLE_TYPE
{
	INDEX_BUFFER = 0,
	VERTEX_BUFFER,
	LAYOUT,
	PIXEL_SHADER,
	VERTEX_SHADER,
	TOPOLOGY,
	TRANSFORMCBUFFER,
	CONSTANT_BUFFER,
	COMPUTE_SHADER,
	TEXTURE,
	SAMPLER,
	SHADER_RESOURCE,
	UNSPECIFIED
};


enum BIND_STAGE {
	INPUT_ASSIMPLY_STAGE = 0,
	VERTEX_SHADER_STAGE,
	PIXEL_SHADER_STAGE,
	COMPUTE_SHADER_STAGE

};

class Bindable
{
private:
	BINDABLE_TYPE BT;
	UINT referenceCount;
public:
	Bindable(BINDABLE_TYPE BT) :BT(BT) , referenceCount(1)
	{
	}

	virtual	void bind(Graphics& gfx) = 0;

	virtual ~Bindable() = default;

	Bindable* AddRefrance() {
		referenceCount++;
		return this;
	}

	Bindable* release() {
		using namespace std;
		referenceCount--;

		if (referenceCount==0)
		{
			this->~Bindable();
			return nullptr;
		}
		return this;
	}

	BINDABLE_TYPE getType() { return BT; }

public:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) {return gfx.getcontext(); }
	static ID3D11Device* GetDevice(Graphics& gfx) {return gfx.getdevice(); }

};


#endif // !BINDABLE