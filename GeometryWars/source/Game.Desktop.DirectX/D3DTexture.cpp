#include "pch.h"
#include "D3DTexture.h"

namespace Library
{
	D3DTexture::D3DTexture(ID3D11Device1& device, ID3D11DeviceContext& context) : mDevice(&device), mContext(&context), mColorTexture(nullptr), mColorSampler(nullptr)
	{
	}

	D3DTexture::~D3DTexture()
	{
		ReleaseObject(mColorTexture);
		ReleaseObject(mColorSampler);

	}
	void D3DTexture::Init(const std::string & imagePath)
	{

		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		ThrowIfFailed(DirectX::CreateWICTextureFromFile(mDevice, mContext, converter.from_bytes(imagePath).c_str(), nullptr, &mColorTexture), "CreateWICTextureFromFile() failed.");

		// Create a texture sampler
		D3D11_SAMPLER_DESC samplerDesc;
		ZeroMemory(&samplerDesc, sizeof(samplerDesc));
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		
		ThrowIfFailed(mDevice->CreateSamplerState(&samplerDesc, &mColorSampler), "ID3D11Device::CreateSamplerState() failed.");
	}
	void D3DTexture::Use(std::uint32_t useAsTextureIndex)
	{
		UNREFERENCED_PARAMETER(useAsTextureIndex);
		mContext->PSSetShaderResources(0, 1, &mColorTexture);
		mContext->PSSetSamplers(0, 1, &mColorSampler);
	}
}