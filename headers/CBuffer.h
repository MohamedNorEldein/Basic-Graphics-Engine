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

	
	VertexConstantBuffer<TransformMat> vcb;
	Drawable *drawableParent;

public:
	TransformCBuffer(Graphics& gfx, UINT Slot =0u)
		:vcb(gfx, Slot), drawableParent(nullptr)
	{
	}

	void setDrawableParent(Drawable* _drawableParent) {
		drawableParent = _drawableParent;
	}

	void bind(Graphics& gfx) {
		vcb.update(gfx, { 

				DirectX::XMMatrixTranspose(
				drawableParent->GetTransform()
		),
			DirectX::XMMatrixTranspose(
				drawableParent->GetTransform()
				*
				gfx.getCamera()
				*
				gfx.getProjection()
		)
			});
		vcb.bind(gfx);

	}

	~TransformCBuffer() {
		//cout("delete TransformCBuffer");
	}

};



#endif // !TRANSFORM_C_BUFFER
