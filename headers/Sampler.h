#pragma once

#include <Bindable.h>

class Sampler :
	public Bindable 
{
private:
	ID3D11SamplerState* psampler;
	UINT slot;
public:
	Sampler(Graphics& gfx, UINT slot=0u) :
		Bindable(BINDABLE_TYPE::SAMPLER),slot(slot)
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;

		gfx.getdevice()->CreateSamplerState(&desc, &psampler);
	}

	void bind(Graphics& gfx) override{
		gfx.getcontext()->PSSetSamplers(slot, 1u, &psampler);
	}

	~Sampler() {
		psampler->Release();
	}


};