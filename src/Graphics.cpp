//
// Created by m7mdn on 7/24/2022.
//

#include <Graphics.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")



void initImGui(Graphics* pgfx, HWND windowhandel) {
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplDX11_Init(pgfx->getdevice(), pgfx->getcontext());
	ImGui_ImplWin32_Init(windowhandel);
}

void Graphics::init(HWND windowHandel, int _width, int _height)
{
	width = _width;
	height = _height;

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

#ifdef _DEBUG
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
#else
	CHECK(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION, &sd,
		&pswapChain, &pdevice,
		nullptr, &pcontext));

#endif

	// create target buffer
	ID3D11Resource* pbackBuffer;
	CHECK(pswapChain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&pbackBuffer));

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
	pcontext->OMSetDepthStencilState(pdepth, 1u);

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

	initImGui(this, windowHandel);
}

Graphics::Graphics(HWND windowHandel,int width, int height)

{
	init(windowHandel, width, height);
	
}

Graphics::Graphics(Window& window) :
	Graphics::Graphics(window.GetHandel(), window.width, window.height)
{
}


Graphics::~Graphics()
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext();

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


	ImGui::EndFrame();

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

void Graphics::setProjection(const DirectX::XMMATRIX& mat) {
	projection = mat;
}

 IDXGISwapChain* Graphics::getswapChain() {
	return pswapChain;
}
 ID3D11DeviceContext* Graphics::getcontext() {
	return pcontext;
}
 ID3D11RenderTargetView* Graphics::getTarget() {
	return pTarget;
}
 ID3D11Device* Graphics::getdevice() {
	return pdevice;
}



DirectX::XMMATRIX& Graphics::getProjection() {

	return projection;
}

DirectX::XMMATRIX Graphics::getCameraProjection()
{
	return camera->getCameraProjection();
}

void Graphics::setCamera(FirstPearsonPerspective* _camera)
{
	camera = _camera;
}


float FirstPearsonPerspective::cx = 0, FirstPearsonPerspective::cy = 0, FirstPearsonPerspective::cz = 0, FirstPearsonPerspective::rx = 0, FirstPearsonPerspective::ry = 0, FirstPearsonPerspective::rz = 0;
using namespace DirectX;

FirstPearsonPerspective::FirstPearsonPerspective(Graphics& gfx) :
	CameraPosition({0,0,0,0}),
	cameraRotation({0,0,0,0}),
	CameraTransilationSpeed(1)
{
	gfx.setCamera(this);
}

DirectX::XMMATRIX FirstPearsonPerspective::getCameraProjection()
{
	using namespace DirectX;

	return
		DirectX::XMMatrixTranslationFromVector(-CameraPosition)
		*
		DirectX::XMMatrixRotationRollPitchYaw(0.0f, -cameraRotation.m128_f32[1], 0.0)
		*
		DirectX::XMMatrixRotationRollPitchYaw(-cameraRotation.m128_f32[0], 0, 0.0);
}

void FirstPearsonPerspective::updateCameraPosition(float x, float y, float z)
{
	CameraPosition += XMVector3Transform(XMVectorSet(x, y, z, 0.0f), XMMatrixRotationRollPitchYawFromVector(cameraRotation));
}

void FirstPearsonPerspective::FirstPearsonPerspective::updateCameraRotation(float x, float y,float z)
{
	cameraRotation += XMVectorSet(x, y, z, 0.0f);
}

void FirstPearsonPerspective::CameraMouseControl(MouseEvents& mouseEvent) {
	cx = 0;
	cy = 0;
	cz = 0;
	rx = 0;
	ry = 0;
	rz = 0;

	if (mouseEvent.getState()&MK_SHIFT) {
		rz = CameraScorollingSpeed * float(mouseEvent.getWheelMove());
	}
	else {
		cz = CameraScorollingSpeed * float(mouseEvent.getWheelMove());
	}

	if (mouseEvent.getState() == (MK_MBUTTON | MK_SHIFT))
	{
		cx -= CameraTransilationSpeed * float(mouseEvent.get_dx());
		cy -= CameraTransilationSpeed * float(mouseEvent.get_dy());

		mouseEvent.handeled();
	}

	if (mouseEvent.getState() & MK_MBUTTON)
	{
		ry += CameraRotationSpeed * float(mouseEvent.get_dx());
		rx += CameraRotationSpeed * float(mouseEvent.get_dy());
		mouseEvent.handeled();

	}

	updateCameraPosition(cx, cy, cz);
	updateCameraRotation(rx, ry,rz);
}

void FirstPearsonPerspective::CameraKeyboardCotrol(KeyBoardEvent keyBoardEvent) {

	cx = 0;
	cy = 0;
	cz = 0;
	rx = 0;
	ry = 0;

	if (keyBoardEvent.isDown('W'))
	{
		cy = CameraTransilationSpeed;

	}
	if (keyBoardEvent.isDown('S'))
	{
		cy = -CameraTransilationSpeed;

	}
	if (keyBoardEvent.isDown('A'))
	{
		cx = -CameraTransilationSpeed;

	}
	if (keyBoardEvent.isDown('D'))
	{
		cx = CameraTransilationSpeed;

	}

	if (keyBoardEvent.isDown(VK_UP))
	{
		rx = -CameraRotationSpeed;

	}
	if (keyBoardEvent.isDown(VK_DOWN))
	{
		rx = CameraRotationSpeed;

	}
	if (keyBoardEvent.isDown(VK_RIGHT))
	{
		ry = CameraRotationSpeed;

	}
	if (keyBoardEvent.isDown(VK_LEFT))
	{
		ry = -CameraRotationSpeed;

	}

	updateCameraPosition(cx, cy, cz);
	updateCameraRotation(rx, ry, rz);
}

ThirdPearsonPerspective::ThirdPearsonPerspective(Graphics& camera) :
	FirstPearsonPerspective(camera)
{

}

void ThirdPearsonPerspective::updateCameraPosition(float x, float y, float z) {
	using namespace DirectX;
	CameraPosition += XMVector3Transform(XMVectorSet(x, y, z, 0.0f), XMMatrixRotationY(cameraRotation.m128_f32[1]) * XMMatrixRotationX(cameraRotation.m128_f32[0]));
}					   

void ThirdPearsonPerspective::updateCameraRotation(float _lattude, float _departure,float nr) {
	
	cameraRotation.m128_f32[2] += nr;
	if (cameraRotation.m128_f32[2] < 0) {
		cameraRotation.m128_f32[2] = 0.0f;
	}

	using namespace DirectX;
	XMVECTOR R = XMVectorSet(0, 0, cameraRotation.m128_f32[2], 0.0f);
	auto M = XMMatrixRotationX(_lattude) * XMMatrixRotationY(_departure) ;
	auto CRM = XMMatrixRotationX(cameraRotation.m128_f32[0])* XMMatrixRotationY(cameraRotation.m128_f32[1]) ;
	auto s = XMVector4Transform(R, 
		(M - XMMatrixIdentity()) * CRM
	);
	CameraPosition += s;
	cameraRotation.m128_f32[0] -= _lattude;
	cameraRotation.m128_f32[1] -= _departure;
}

void FirstPearsonPerspective::GUIcontrol() {
	
	if (ImGui::Begin("camera", nullptr, (ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))) {
		ImGui::Text("For Camera Transilation speed in System control");
		ImGui::SliderFloat("cammera speed", &CameraTransilationSpeed, 0, 10);
		ImGui::Text("Camera GUIcontrol");
		ImGui::SliderFloat3("cammera Position", CameraPosition.m128_f32, -1000.0f, 1000.0f);
		ImGui::SliderAngle("cammera Lattiude", &cameraRotation.m128_f32[0], -180.0f, 180.0f);
		ImGui::SliderAngle("cammera Departure", &cameraRotation.m128_f32[1], -180.0f, 180.0f);
		ImGui::SliderFloat("cammera Radius", &cameraRotation.m128_f32[2], 0.0f, 1000.0f);

	}
	ImGui::End();
	
}

