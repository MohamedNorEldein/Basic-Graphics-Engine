#pragma once

#include "Bindable.h"

class ComputeShader :
	public Bindable
{
private:

	ID3D11ComputeShader* cs;
	ID3DBlob* pBlop = 0;

public:
	ComputeShader(Graphics& gfx, const wchar_t* src,bool compiled=true)
		:Bindable(BINDABLE_TYPE::COMPUTE_SHADER)
	{
		
		if (compiled) {
			CHECK(D3DReadFileToBlob(src, &pBlop));
			gfx.getdevice()->CreateComputeShader(pBlop->GetBufferPointer(), pBlop->GetBufferSize(), NULL, &cs);

		}
		else {
#ifdef _DEBUG
			CHECK(D3DCompileFromFile(src, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", D3DCOMPILE_DEBUG, 0, &pBlop, NULL));
#else
			CHECK(D3DCompileFromFile(src, NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_0", 0, 0, &pBlop, NULL));

#endif // _DEBUG

			CHECK(gfx.getdevice()->CreateComputeShader(pBlop->GetBufferPointer(), pBlop->GetBufferSize(), NULL, &cs));

		}
	}

	void bind(Graphics& gfx)override {
		gfx.getcontext()->CSSetShader(cs, 0, 0);
	}

	~ComputeShader() {
		pBlop->Release();
		cs->Release();
	}

};

