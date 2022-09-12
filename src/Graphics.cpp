//
// Created by m7mdn on 7/24/2022.
//

#include <Graphics.h>



Graphics::Graphics(HWND windowHandel,int width, int height)
	:	CameraPosition(0,0,128),
	cameraRotation(0,0,1),
	width(width),
	height(height)

{

	DXGI_SWAP_CHAIN_DESC sd = { 0 };
	// buffer
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

 	sd.BufferCount = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	  
	sd.Windowed = true;
	sd.OutputWindow = windowHandel;

	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
	sd.Flags = 0; 


	CHECK(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION, &sd,
		&pswapChain, &pdevice,
		nullptr, &pcontext));
	
	// create target buffer
	ID3D11Resource* pbackBuffer;
	CHECK(pswapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**) & pbackBuffer));
	
	CHECK(pdevice->CreateRenderTargetView(pbackBuffer, nullptr, &pTarget));
	
	CHECK(pbackBuffer->Release());


	setViewPort(0, 0, width, height);


	// Z buffer 

	ID3D11DepthStencilState* pdepth;

	D3D11_DEPTH_STENCIL_DESC dDesc = { 0 };

	dDesc.DepthEnable = true;
	dDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dDesc.DepthFunc = D3D11_COMPARISON_LESS;
	

	pdevice->CreateDepthStencilState(&dDesc, &pdepth);
	pcontext->OMSetDepthStencilState(pdepth,1u);

	ID3D11Texture2D* pdepthTex = 0;
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1u;
	texDesc.ArraySize = 1u;
	texDesc.Format = DXGI_FORMAT_D32_FLOAT;
	texDesc.SampleDesc.Count = 1u;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	pdevice->CreateTexture2D(&texDesc, 0, &pdepthTex);

	
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvSDesc = {};

	dsvSDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvSDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvSDesc.Texture2D.MipSlice = 0u;

	pdevice->CreateDepthStencilView(pdepthTex, &dsvSDesc, &pdsv);

	pcontext->OMSetRenderTargets(1u, &pTarget, pdsv);
	
}

Graphics::~Graphics()
{
	if (!pTarget)
		pTarget->Release();
	if (!pswapChain)
		pswapChain->Release();
	if (!pcontext)
		pcontext->Release();
	if (!pdevice)
		pdevice->Release();
	if (!pdsv)
		pdsv->Release();
}

void Graphics::BegainFrame()
{

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void Graphics::EndFrame()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// put buffer on screen;
	CHECK(pswapChain->Present(1u, 0u));
}

void Graphics::clearBuffer(float red, float green, float blue)
{
	float color[4] = { red,green,blue,1.0 };
	pcontext->ClearRenderTargetView(pTarget, color);
	pcontext->ClearDepthStencilView(pdsv, D3D11_CLEAR_DEPTH, 1.0, 0u);
}

void Graphics::setViewPort(int x, int y, int width, int height)
{
	//--------------------
	 //veiw port
	D3D11_VIEWPORT vp = { };
	vp.Width =	width ;
	vp.Height = height;
	vp.MaxDepth = 1;
	vp.TopLeftX = x;
	vp.TopLeftY = y;

	pcontext->RSSetViewports(1, &vp);
}

// test 

struct constBuffer2 {
	float r, g, b, a;
};

struct constBuffer
{
	DirectX::XMMATRIX Tmat;

};



void Graphics::CreatetestTriangle() {

	// IA
	// vertex buffer

	struct Vertex {
		
			float x, y, z;
		
	};

	const Vertex vertexBufferData[] =
	{

	{ -3.0f, -3.0f, -3.0f},
	{ 3.0f, -3.0f, -3.0f},

	{ -3.0f, 3.0f, -3.0f},
	{ 3.0f, 3.0f, -3.0f },

	{ -3.0f, -3.0f, 3.0f},
	{ 3.0f, -3.0f, 3.0f},

	{ -3.0f, 3.0f, 3.0f},
	{ 3.0f, 3.0f, 3.0f},

	};

	D3D11_SUBRESOURCE_DATA vdata = { 0 };
	vdata.pSysMem = vertexBufferData;

	D3D11_BUFFER_DESC BufferDesc = { 0 };

	BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	 
	BufferDesc.CPUAccessFlags = 0u;
	BufferDesc.MiscFlags = 0u;

	BufferDesc.StructureByteStride = sizeof(Vertex);
	BufferDesc.ByteWidth = sizeof(vertexBufferData);


	ID3D11Buffer* pVertexBuffer=0;
 
	CHECK(pdevice->CreateBuffer(&BufferDesc, &vdata, &pVertexBuffer));

	const UINT stride = sizeof(Vertex); 
	const UINT offset = 0u;

	pcontext->IASetVertexBuffers(0u, 1u, &pVertexBuffer, &stride, &offset);


	//-----------------------------------------------------
	//index buffer 
	unsigned short indexBufferData[] = {
		0,2,1,	2,3,1,
		1,3,5,	3,7,5,
		2,6,3,	3,6,7,
		4,5,7,	4,7,6,
		0,4,2,	2,4,6,
		0,1,4,	1,5,4
	};

	ID3D11Buffer* pindexBuffer = 0;
	D3D11_SUBRESOURCE_DATA idata = { 0 };
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };

	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	indexBufferDesc.ByteWidth = sizeof(indexBufferData);
	cout(sizeof(indexBufferData) / sizeof(unsigned short));
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;

	idata.pSysMem = indexBufferData;
	
	pdevice->CreateBuffer(&indexBufferDesc, &idata, &pindexBuffer);
	pcontext->IASetIndexBuffer(pindexBuffer,DXGI_FORMAT_R16_UINT,0);


	pcontext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);



	//-----------------------------
	// pixel shader
	ID3D11PixelShader* pPixelShaders[1] ={0};
	ID3DBlob* pBlob;

	//ID3DBlob* 
	CHECK(D3DReadFileToBlob(L"PixelShader1.cso", &pBlob));
	CHECK(pdevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), 0, pPixelShaders));
	pcontext->PSSetShader(pPixelShaders[0], 0, 0);

	//--------------------------
	//vertex shader 
	ID3D11VertexShader* pvertexShaders[1]={0};

	CHECK(D3DReadFileToBlob(L"VertexShader1.cso", &pBlob));

	CHECK(pdevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(),0,pvertexShaders));
	pcontext->VSSetShader(pvertexShaders[0], 0, 0);
	

	//--------------------------------
	// vertex input layout object
	ID3D11InputLayout* pInputLayout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{ 
	//	 name		symantic index	formate					slot =0		ofsset	input type					step rate
		{"POSITION",0,	DXGI_FORMAT_R32G32B32_FLOAT		,0,			0,		D3D11_INPUT_PER_VERTEX_DATA,	0},
	//	{"COLOR",	0,	DXGI_FORMAT_R8G8B8A8_UNORM		,0,		12u,		D3D11_INPUT_PER_VERTEX_DATA,	0}

	};

	CHECK(pdevice->CreateInputLayout(ied, std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout));
	
	pcontext->IASetInputLayout(pInputLayout);


	//-------------------------
	//render target
	
	pcontext->OMSetRenderTargets(1u, &pTarget, nullptr);

	//--------------------
	//veiw port
	D3D11_VIEWPORT vp = { };
	vp.Width = 640;
	vp.Height = 480;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	pcontext->RSSetViewports(1, &vp);

	
	//out(L"all done");

}


DirectX::XMMATRIX Graphics::getProjection() {

	using namespace DirectX;

	return projection;
}

DirectX::XMMATRIX Graphics::getCamera()
{
	using namespace DirectX;


	return
		DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&CameraPosition))
		*
		DirectX::XMMatrixRotationRollPitchYaw(-cameraRotation.y, -cameraRotation.x, 0);
}


void Graphics::updatePosition(float z) {
	CameraPosition.z += z;
}

void Graphics::updateScale(float z)
{
	cameraRotation.z += 100 * z;
}

void Graphics::updateCameraRotation(float x, float y) {
	cameraRotation.x = x;
	cameraRotation.y = y;

}


void Graphics::updateCameraRotation(int x, int y) 
{
	
	updateCameraRotation(float(x)*0.1f / width, float(y)*0.1f / height);
}

void Graphics::updateCameraPosition(float x, float y) {
	CameraPosition.x = x;
	CameraPosition.y = y;

}

