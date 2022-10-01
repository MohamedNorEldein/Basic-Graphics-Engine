
#include <App.h>

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
	
	{
		Window win(L"compute pipeline",640,480);
		Graphics gfx(win);
		ComputeShader cs(gfx, L"Shaders\\ComputeShader.hlsl", false);
		ID3D11Buffer* pinputBuffer = 0;
		ID3D11ShaderResourceView* pinputView = 0;
		int inAr[2000] = { 0 };
	

		D3D11_SUBRESOURCE_DATA vdata = { 0 };
		vdata.pSysMem = inAr;

		D3D11_BUFFER_DESC BufferDesc = { 0 };

		BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		BufferDesc.CPUAccessFlags = 0u;
		BufferDesc.MiscFlags = 0u;

		BufferDesc.StructureByteStride = sizeof(int);
		BufferDesc.ByteWidth = 2000 * sizeof(int);

		CHECK(gfx.getdevice()->CreateBuffer(&BufferDesc, &vdata, &pinputBuffer));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvd;

		srvd.Format = DXGI_FORMAT_R32_UINT;
		srvd.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		srvd.Texture2D.MipLevels = 1u;
		srvd.Texture2D.MostDetailedMip = 0u;

		CHECK(gfx.getdevice()->CreateShaderResourceView(pinputBuffer, &srvd, &pinputView));



		gfx.getcontext()->CSSetShaderResources(0, 1,&pinputView);
		cs.bind(gfx);
		gfx.getcontext()->Dispatch(100, 1, 1);

	}
	std::cin.get();
	FreeConsole();
	return 0;
}


