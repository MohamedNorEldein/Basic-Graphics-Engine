#include "constantBuffer.h"
#include "Drawable.h"


/*
* problems with camera rotaion
* need to be figuered
*/

#ifndef TRANSFORM_C_BUFFER
#define TRANSFORM_C_BUFFER


class TransformCBuffer :
	public Bindable
{
private:

	__declspec(align(16))
		struct  TransformMat
	{
		DirectX::XMMATRIX nTr;
		DirectX::XMMATRIX Tr;
	};

	VertexConstantBuffer vcb;
	

public:
	TransformCBuffer(Graphics& gfx, UINT Slot = 0u)
		:vcb(gfx, Slot, sizeof(TransformMat)), Bindable(_TransforCBuffer)
	{
	}

	void update(Graphics& gfx,const DirectX::XMMATRIX& Tr) {
		vcb.update(gfx, 
		TransformMat{
				DirectX::XMMatrixTranspose(Tr)
			,
			DirectX::XMMatrixTranspose(
				Tr
				*
				gfx.getCamera()
				*
				gfx.getProjection())
		});
	}

	void bind(Graphics& gfx) {

		vcb.bind(gfx);

	}

	~TransformCBuffer() {
		//cout("delete TransformCBuffer");
	}

};



#endif // !TRANSFORM_C_BUFFER
