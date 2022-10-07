#include "ComputePipeLine.h"

//
// Created by m7mdn on 7/24/2022.
//

#include <Graphics.h>

#pragma comment(lib, "user32")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")



ComputePipeLine::ComputePipeLine()

{
#ifdef _DEBUG

	CHECK(D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		0,
		D3D11_SDK_VERSION, &pdevice, 0, &pcontext));
#else

	CHECK(D3D11CreateDevice(nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION, &pdevice, 0, &pcontext));

#endif
}


ComputePipeLine::~ComputePipeLine()
{
	
	if (!pcontext)
		pcontext->Release();
	if (!pdevice)
		pdevice->Release();
	
}

ID3D11DeviceContext* ComputePipeLine::getcontext() {
	return pcontext;
}

ID3D11Device* ComputePipeLine::getdevice() {
	return pdevice;
}


