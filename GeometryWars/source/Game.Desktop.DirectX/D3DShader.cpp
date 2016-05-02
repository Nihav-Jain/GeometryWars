#include "pch.h"
#include "D3DShader.h"
#include "Utility.h"

namespace Library
{
	D3DShader::D3DShader(ID3D11Device1& device, ID3D11DeviceContext& context) : mDevice(&device), mContext(&context),
		mVertexShader(nullptr), mPixelShader(nullptr)
	{

		std::vector<char> compiledVertexShader;
		Utility::LoadBinaryFile(L"VertexShader.cso", compiledVertexShader);
		ThrowIfFailed(device.CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");
	}


	D3DShader::~D3DShader()
	{
	}
	void D3DShader::Init(const std::string & vPath, const std::string & fPath)
	{
		UNREFERENCED_PARAMETER(vPath);
		UNREFERENCED_PARAMETER(fPath);
		std::vector<char> compiledVertexShader;
		//Utility::LoadBinaryFile(std::wstring(vPath.begin(), vPath.end()), compiledVertexShader);
		Utility::LoadBinaryFile(L"VertexShader.cso", compiledVertexShader);
		ThrowIfFailed(mDevice->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		std::vector<char> compiledPixelShader;
		//Utility::LoadBinaryFile(std::wstring(fPath.begin(), fPath.end()), compiledPixelShader);
		Utility::LoadBinaryFile(L"PixelShader.cso", compiledPixelShader);
		ThrowIfFailed(mDevice->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &mPixelShader), "ID3D11Device::CreatedPixelShader() failed.");
	}
	void D3DShader::Use()
	{
		mContext->VSSetShader(mVertexShader, nullptr, 0);
		mContext->PSSetShader(mPixelShader, nullptr, 0);

	}
	void D3DShader::SetMatrix4(const std::string & name, const glm::mat4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}
	void D3DShader::SetVector4(const std::string & name, const glm::vec4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
	}
}