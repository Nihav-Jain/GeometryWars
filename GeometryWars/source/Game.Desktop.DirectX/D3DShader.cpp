#include "pch.h"
#include "D3DShader.h"
#include "Utility.h"

namespace Library
{
	D3DShader::D3DShader(ID3D11Device1& device, ID3D11DeviceContext& context) : mDevice(&device), mContext(&context),
		mVertexShader(nullptr), mGeometryShader(nullptr), mPixelShader(nullptr), mInputLayout(nullptr), mConstantGeometryBuffer(nullptr), mConstantPixelBuffer(nullptr), mGeoBufferInstance()
	{
	}


	D3DShader::~D3DShader()
	{
		ReleaseObject(mVertexShader);
		ReleaseObject(mGeometryShader);
		ReleaseObject(mPixelShader);
		ReleaseObject(mInputLayout);
		ReleaseObject(mConstantGeometryBuffer);
		ReleaseObject(mConstantPixelBuffer);
	}
	void D3DShader::Init(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		UNREFERENCED_PARAMETER(vPath);
		UNREFERENCED_PARAMETER(gPath);
		UNREFERENCED_PARAMETER(fPath);
		std::vector<char> compiledVertexShader;
		//Utility::LoadBinaryFile(std::wstring(vPath.begin(), vPath.end()), compiledVertexShader);
		Utility::LoadBinaryFile(L"VertexShader.cso", compiledVertexShader);
		ThrowIfFailed(mDevice->CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");

		// Load a compiled pixel shader
		std::vector<char> compiledGeometryShader;
		//Utility::LoadBinaryFile(std::wstring(fPath.begin(), fPath.end()), compiledPixelShader);
		Utility::LoadBinaryFile(L"GeometryShader.cso", compiledGeometryShader);
		ThrowIfFailed(mDevice->CreateGeometryShader(&compiledGeometryShader[0], compiledGeometryShader.size(), nullptr, &mGeometryShader), "ID3D11Device::CreatedGeometryShader() failed.");

		std::vector<char> compiledPixelShader;
		//Utility::LoadBinaryFile(std::wstring(fPath.begin(), fPath.end()), compiledPixelShader);
		Utility::LoadBinaryFile(L"PixelShader.cso", compiledPixelShader);
		ThrowIfFailed(mDevice->CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &mPixelShader), "ID3D11Device::CreatedPixelShader() failed.");


		D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};
		ThrowIfFailed(mDevice->CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &mInputLayout), "ID3D11Device::CreateInputLayout() failed.");

		D3D11_BUFFER_DESC constantBufferDesc;
		ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		constantBufferDesc.ByteWidth = sizeof(CGeometryBufferPerObject);
		ThrowIfFailed(mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantGeometryBuffer), "ID3D11Device::CreateBuffer() failed.");

		constantBufferDesc.ByteWidth = sizeof(CPixelBufferPerObject);
		ThrowIfFailed(mDevice->CreateBuffer(&constantBufferDesc, nullptr, &mConstantPixelBuffer), "ID3D11Device::CreateBuffer() failed.");
	}
	void D3DShader::Use()
	{
		mContext->VSSetShader(mVertexShader, nullptr, 0);
		mContext->GSSetShader(mGeometryShader, nullptr, 0);
		mContext->PSSetShader(mPixelShader, nullptr, 0);

		mContext->IASetInputLayout(mInputLayout);
		//mContext->GSSetConstantBuffers(0, 1, &mConstantGeometryBuffer);
		//mContext->PSSetConstantBuffers(0, 1, &mConstantPixelBuffer);

		//CPixelBufferPerObject pBuff{{ 1, 0, 0, 1 }};
		//gBuff.width = 0.01f;
		//XMStoreFloat4x4(&gBuff.WorldViewProjection, XMMatrixOrthographicRH(800, 600, -1, 1));
		//XMStoreFloat4x4(&mGeoBufferInstance.WorldViewProjection, XMMatrixScaling(0.1f,0.1f,1));

		
		//mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &gBuff, 0, 0);
		mContext->GSSetConstantBuffers(0, 1, &mConstantGeometryBuffer);

		//mContext->UpdateSubresource(mConstantPixelBuffer, 0, nullptr, &pBuff, 0, 0);
		mContext->PSSetConstantBuffers(0, 1, &mConstantPixelBuffer);
	}
	void D3DShader::SetMatrix4(const std::string & name, const glm::mat4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		//mGeoBufferInstance
		mGeoBufferInstance.WorldViewProjection =
			XMFLOAT4X4(
				value[0][0], value[1][0], value[2][0], value[3][0],
				value[0][1], value[1][1], value[2][1], value[3][1],
				value[0][2], value[1][2], value[2][2], value[3][2],
				value[0][3], value[1][3], value[2][3], value[3][3]);
		mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &mGeoBufferInstance, 0, 0);
	}
	void D3DShader::SetVector4(const std::string & name, const glm::vec4 & value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
		mContext->UpdateSubresource(mConstantPixelBuffer, 0, nullptr, &value, 0, 0);
	}
	void D3DShader::SetFloat(const std::string & name, float value)
	{
		UNREFERENCED_PARAMETER(name);
		UNREFERENCED_PARAMETER(value);
		mGeoBufferInstance.width = value;
		mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &mGeoBufferInstance, 0, 0);
	}
}