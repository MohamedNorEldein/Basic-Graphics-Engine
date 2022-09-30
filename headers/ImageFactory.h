#pragma once

#include <dxgiformat.h>
#include <wincodec.h>
#include <d3d11.h>
#include <iostream>

#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))


struct WICConvert
{
	GUID        source;
	GUID        target;
};

struct WICTranslate
{
	GUID                wic;
	DXGI_FORMAT         format;
};

class Image {

private:
	UINT twidth, theight;
	size_t rowPitch, imageSize;
	DXGI_FORMAT format;
	byte* data;
private:
	static IWICImagingFactory* s_Factory;
private:
	HRESULT createColorArray(ID3D11Device* d3dDevice, IWICBitmapFrameDecode* frame, size_t maxsize);

public:
	Image(const wchar_t* fileName, _In_ ID3D11Device* d3dDevice, _In_ size_t maxsize = 0u);
	~Image();
public:
	UINT getWidth() { return twidth; }
	UINT getHeight() { return theight; }
	size_t getRowPitch() { return rowPitch; };
	size_t getImageSize() { return imageSize; }
	DXGI_FORMAT getFormat() { return format; }
	byte* getData() { return data; }

private:

	static IWICImagingFactory* _GetWIC();
	static DXGI_FORMAT _WICToDXGI(const GUID& guid);
	static size_t _WICBitsPerPixel(REFGUID targetGuid);

};




