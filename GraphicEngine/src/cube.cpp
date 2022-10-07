#pragma once
#include "Cube.h"


#include "constantBuffer.h"
#include "InputLayout.h"
#include "PixelShader.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "IndexBuffer.h"
#include "PrimativeTopology.h"
#include "Texture.h"
#include "Sampler.h"

bool Cube::init = false;
Drawable Cube::dc;


std::vector<Cube::vertex> vBuffData = {
	// vertex position				normals				texture coordinates
{	{ -1.0f , -1.0f, -1.0f },	{ 0 ,  0, -1   }	, {0.0f	,	0.0f} }, //  0 near side
{	{   1.0f , -1.0f, -1.0f },	{ 0 ,  0, -1   }	, {1.0f	,	0.0f} }, //  1
{	{ -1.0f ,  1.0f, -1.0f },	{ 0 ,  0, -1   }	, {0.0f	,	1.0f} }, //  2
{	{   1.0f ,  1.0f, -1.0f },	{ 0 ,  0, -1   }	, {1.0f	,	1.0f} }, //  3
{	{ -1.0f , -1.0f,   1.0f },	{ 0 ,  0,  1   }	, {0.0f	,	0.0f} }, //  4 far side
{	{   1.0f , -1.0f,   1.0f },	{ 0 ,  0,  1   }	, {1.0f	,	0.0f} }, //  5
{	{ -1.0f ,  1.0f,   1.0f },	{ 0 ,  0,  1   }	, {0.0f	,	1.0f} }, //  6
{	{   1.0f ,  1.0f,   1.0f },	{ 0 ,  0,  1   }	, {1.0f	,	1.0f} }, //  7
{	{ -1.0f , -1.0f, -1.0f },	{-1 ,  0,  0   }	, {0.0f	,	0.0f} }, //  8 left side
{	{ -1.0f ,  1.0f, -1.0f },	{-1 ,  0,  0   }	, {1.0f	,	0.0f} }, //  9
{	{ -1.0f , -1.0f,   1.0f },	{-1 ,  0,  0   }	, {0.0f	,	1.0f} }, //  10
{	{ -1.0f ,  1.0f,   1.0f },	{-1 ,  0,  0   }	, {1.0f	,	1.0f} }, //  11
{	{   1.0f , -1.0f, -1.0f },	{1  ,  0,  0   }	, {0.0f	,	0.0f} }, //  12 right side
{	{   1.0f ,  1.0f, -1.0f },	{1  ,  0,  0   }	, {1.0f	,	0.0f} }, //  13
{	{   1.0f , -1.0f,   1.0f },	{1  ,  0,  0   }	, {0.0f	,	1.0f} }, //  14
{	{   1.0f ,  1.0f,   1.0f },	{1  ,  0,  0   }	, {1.0f	,	1.0f} }, //  15
{	{ -1.0f , -1.0f, -1.0f },	{0  , -1,  0   }	, {0.0f	,	0.0f} }, //  16 bottom side
{	{   1.0f , -1.0f, -1.0f },	{0  , -1,  0   }	, {1.0f	,	0.0f} }, //  17
{	{ -1.0f , -1.0f,   1.0f },	{0  , -1,  0   }	, {0.0f	,	1.0f} }, //  18
{	{   1.0f , -1.0f,   1.0f },	{0  , -1,  0   }	, {1.0f	,	1.0f} }, //  19
{	{ -1.0f ,  1.0f, -1.0f },	{0  ,  1,  0   }	, {0.0f	,	0.0f} }, //  20 top side
{	{   1.0f ,  1.0f, -1.0f },	{0  ,  1,  0   }	, {1.0f	,	0.0f} }, //  21
{	{ -1.0f ,  1.0f,   1.0f },	{0  ,  1,  0   }	, {0.0f	,	1.0f} }, //  22
{	{   1.0f ,  1.0f,   1.0f },	{0  ,  1,  0   }	, {1.0f	,	1.0f} } //  23
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

D3D11_INPUT_ELEMENT_DESC ied[] = {
	{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, sizeof(DirectX::XMFLOAT3), D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(DirectX::XMFLOAT3) * 2, D3D11_INPUT_PER_VERTEX_DATA, 0}

};


void Cube::generate() {
	printf("loading static bindables across all the cube class\n");
	VertexShader* vs = new VertexShader(gfx, L"shaders\\CubeVertexShader.hlsl",false);
	//Cube::pcb = new PixelConstantBuffer(gfx, 1u, sizeof(Cube::COLOR));
	dc.AddBindable(new IndexBuffer(gfx, indeces));
	dc.AddBindable(new VertexBuffer(gfx, vBuffData));
	dc.AddBindable(vs);
	dc.AddBindable(new PixelShader(gfx, L"shaders\\CubePixelShader.hlsl",false));
	dc.AddBindable(new InputLayout(gfx, ied,3, vs->getpBlob()));
///	dc.AddBindable(Cube::pcb);
	tr = new TransformCBuffer(gfx);
	dc.AddBindable(tr);
	dc.AddBindable(new PrimativeTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	dc.AddBindable(new Texture(gfx, L"Models src data\\Great_Wave_off_Kanagawa2.jpg"));
	dc.AddBindable(new Sampler(gfx, 0u));
}

Cube::Cube(Graphics& gfx) :
	gfx(gfx), pos(0, 0, 0), theta(0, 0, 0), scale(1, 1, 1), c({ 1,1,1 })
{
	if (!init) {
		generate();
		init = !init;
	}
	
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

TransformCBuffer* Cube::tr;
//PixelConstantBuffer* Cube::pcb;
