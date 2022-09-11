#pragma once

#include "IndexBuffer.h"


/* interface class
* which only made of pure virtual functions
*/


enum  bindableType
{
	_unspecified = 0, _Indexbuffer, _VertexBuffer, _Layout, _PixelShader, _VertexShader, _TransforCBuffer, _PixelConstantBuffer, _VertexConstantBuffer
};


class Drawable 
{
public:
	virtual void setPrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY pt) = 0;

	virtual void AddBindable(Bindable* bindable, bindableType BT = _unspecified)=0;
	
	virtual void Draw(Graphics& gfx) = 0;
	virtual DirectX::XMMATRIX GetTransform() = 0;

};

