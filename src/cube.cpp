#pragma once
#include "Cube.h"


bool Cube::init = false;
Drawable Cube::dc;


std::vector<Cube::vertex> vBuffData = {

{{ -1.0f , -1.0f, -1.0f },{ 0 ,  0, -1   }}, //  0 near side
{{   1.0f , -1.0f, -1.0f },{ 0 ,  0, -1   }}, //  1
{{ -1.0f ,  1.0f, -1.0f },{ 0 ,  0, -1   }}, //  2
{{   1.0f ,  1.0f, -1.0f },{ 0 ,  0, -1   }}, //  3
{{ -1.0f , -1.0f,   1.0f },{ 0 ,  0,  1   }}, //  4 far side
{{   1.0f , -1.0f,   1.0f },{ 0 ,  0,  1   }}, //  5
{{ -1.0f ,  1.0f,   1.0f },{ 0 ,  0,  1   }}, //  6
{{   1.0f ,  1.0f,   1.0f },{ 0 ,  0,  1   }}, //  7
{{ -1.0f , -1.0f, -1.0f },{-1 ,  0,  0   }}, //  8 left side
{{ -1.0f ,  1.0f, -1.0f },{-1 ,  0,  0   }}, //  9
{{ -1.0f , -1.0f,   1.0f },{-1 ,  0,  0   }}, //  10
{{ -1.0f ,  1.0f,   1.0f },{-1 ,  0,  0   }}, //  11
{{   1.0f , -1.0f, -1.0f },{1  ,  0,  0   }}, //  12 right side
{{   1.0f ,  1.0f, -1.0f },{1  ,  0,  0   }}, //  13
{{   1.0f , -1.0f,   1.0f },{1  ,  0,  0   }}, //  14
{{   1.0f ,  1.0f,   1.0f },{1  ,  0,  0   }}, //  15
{{ -1.0f , -1.0f, -1.0f },{0  , -1,  0   }}, //  16 bottom side
{{   1.0f , -1.0f, -1.0f },{0  , -1,  0   }}, //  17
{{ -1.0f , -1.0f,   1.0f },{0  , -1,  0   }}, //  18
{{   1.0f , -1.0f,   1.0f },{0  , -1,  0   }}, //  19
{{ -1.0f ,  1.0f, -1.0f },{0  ,  1,  0   }}, //  20 top side
{{   1.0f ,  1.0f, -1.0f },{0  ,  1,  0   }}, //  21
{{ -1.0f ,  1.0f,   1.0f },{0  ,  1,  0   }}, //  22
{{   1.0f ,  1.0f,   1.0f },{0  ,  1,  0   }} //  23
};

// index buffer data
std::vector<unsigned short> indeces =
{
0,2, 1,    2,3,1,
4,5, 7,    4,7,6,
8,10, 9,  10,11,9,
12,13,15, 12,15,14,
16,17,18, 18,17,19,
20,23,21, 20,22,23

};

std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0}

};

void Cube::generate() {
	printf("loading static bindables across all the cube class\n");
	VertexShader* vs = new VertexShader(gfx, L"shaders\\AssVertexShader.cso");
	Cube::pcb = new PixelConstantBuffer(gfx, 1u, sizeof(Cube::COLOR));
	dc.AddBindable(new IndexBuffer(gfx, indeces));
	dc.AddBindable(new VertexBuffer(gfx, vBuffData));
	dc.AddBindable(vs);
	dc.AddBindable(new PixelShader(gfx, L"shaders\\AssPixelShader.cso"));
	dc.AddBindable(new InputLayout(gfx, ied, vs->getpBlob()));
	dc.AddBindable(Cube::pcb);
	dc.AddBindable(new TransformCBuffer(gfx));
	dc.AddBindable(new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	dc.AddBindable(new PixelConstantBuffer(gfx, 1u, sizeof(c)));
}

Cube::Cube(Graphics& gfx) :
	gfx(gfx), pos(0, 0, 0), theta(0, 0, 0), scale(1, 1, 1), c({ 1,1,1 })
{
	if (!init) {
		generate();
		init = !init;
	}
	tr = (TransformCBuffer*)(dc[_TransforCBuffer]);
	pcb = (PixelConstantBuffer*)(dc[_PixelConstantBuffer]);
}

void Cube::Draw()
{
	using namespace DirectX;
	tr->update(gfx,
		XMMatrixScalingFromVector(XMLoadFloat3(&scale))
		*
		XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&theta))
		*
		XMMatrixTranslationFromVector(XMLoadFloat3(&pos)));
	//pcb->update(gfx, c);

	dc.Draw(gfx);

}

PixelConstantBuffer* Cube::pcb;
