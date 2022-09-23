
#include<Graphics.h>
#include <Vector>


#ifndef BINDABLE
#define BINDABLE



enum  bindableType
{
	_Indexbuffer = 0, _VertexBuffer,
	_Layout, _PixelShader, _VertexShader, _PrimativeTopology, _TransforCBuffer ,  _PixelConstantBuffer, _VertexConstantBuffer ,_unspecified

};


class Bindable
{
private:
	bindableType BT;
public:
	Bindable(bindableType BT) :BT(BT) 
	{
	}

	virtual	void bind(Graphics& gfx) = 0;

	virtual ~Bindable() = default;

	bindableType getType() { return BT; }

public:
	static ID3D11DeviceContext* GetContext(Graphics& gfx) {return gfx.pcontext;}
	static ID3D11Device* GetDevice(Graphics& gfx) {return gfx.pdevice;}

};


#endif // !BINDABLE