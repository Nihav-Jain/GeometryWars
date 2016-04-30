#include "pch.h"
#include "PolygonRenderer.h"
#include "GameException.h"
#include <vector>
#include "Utility.h"

using namespace Library;
PolygonRenderer::PolygonRenderer(ID3D11Device1 & device, ID3D11DeviceContext & context): 
	mDevice(&device), mContext(&context), mPixelShader(nullptr), mVertexShader(nullptr), mGeomShader(nullptr),
	mVertexBuffer(nullptr), mIndexBuffer(nullptr), mConstantVertexBuffer(nullptr), mConstantGeometryBuffer(nullptr),
mWorldMatrix(XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f))
{
	////////////////////Move this to util

	WCHAR buffer[MAX_PATH];
	GetModuleFileName(nullptr, buffer, MAX_PATH);
	PathRemoveFileSpec(buffer);
	SetCurrentDirectory(std::wstring(buffer).c_str());
	///////////////////
	// Load a compiled vertex shader
	std::vector<char> compiledVertexShader;
	Utility::LoadBinaryFile(L"VertexShader.cso", compiledVertexShader);
	ThrowIfFailed(device.CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &mVertexShader), "ID3D11Device::CreatedVertexShader() failed.");

	// Load a compiled pixel shader
	std::vector<char> compiledPixelShader;
	Utility::LoadBinaryFile(L"PixelShader.cso", compiledPixelShader);
	ThrowIfFailed(device.CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &mPixelShader), "ID3D11Device::CreatedPixelShader() failed.");

	// Load a compiled geom shader
	std::vector<char> compiledGeomShader;
	Utility::LoadBinaryFile(L"GeometryShader.cso", compiledGeomShader);
	ThrowIfFailed(device.CreateGeometryShader(&compiledGeomShader[0], compiledGeomShader.size(), nullptr, &mGeomShader), "ID3D11Device::CreatedGeometryShader() failed.");

	// Create an input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(device.CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &mInputLayout), "ID3D11Device::CreateInputLayout() failed.");

	// Create a vertex buffer
	VertexPositionColor vertices[] =
	{
		VertexPositionColor { XMFLOAT4(  0, -200, 0, 1), XMFLOAT4(1, 0, 0, 0) },
		VertexPositionColor { XMFLOAT4(-50, -150, 0, 1), XMFLOAT4(0, 1, 0, 0) },
		VertexPositionColor { XMFLOAT4(  0, -100, 0, 1), XMFLOAT4(0, 0, 1, 0) },
		VertexPositionColor { XMFLOAT4( 50, -150, 0, 1), XMFLOAT4(1, 1, 0, 0) },
		VertexPositionColor { XMFLOAT4(  0,    0, 0, 1), XMFLOAT4(0, 1, 1, 0) },
		VertexPositionColor { XMFLOAT4(-50,    0, 0, 1), XMFLOAT4(1, 0, 1, 0) },
		VertexPositionColor { XMFLOAT4( 50,    0, 0, 1), XMFLOAT4(1, 1, 1, 0) },
		VertexPositionColor { XMFLOAT4(-50,  100, 0, 1), XMFLOAT4(0, 0, 0, 0) },
		VertexPositionColor { XMFLOAT4( 50,  100, 0, 1), XMFLOAT4(0, 0, 0, 0) }
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * ARRAYSIZE(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = vertices;
	ThrowIfFailed(device.CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer), "ID3D11Device::CreateBuffer() failed.");

	// Create an index buffer
	UINT indices[] =
	{
		0, 1,	1, 2,	2, 3,	3, 0, //head
		2, 4,	2, 5,	2, 6, //body
		4, 7,	4, 8	//legs
	};

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexSubResourceData;
	ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
	indexSubResourceData.pSysMem = indices;
	ThrowIfFailed(device.CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer), "ID3D11Device::CreateBuffer() failed.");

	// Create a constant buffers
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	constantBufferDesc.ByteWidth = sizeof(CGeometryBufferPerObject);
	ThrowIfFailed(device.CreateBuffer(&constantBufferDesc, nullptr, &mConstantGeometryBuffer), "ID3D11Device::CreateBuffer() failed.");

	constantBufferDesc.ByteWidth = sizeof(CVertexBufferPerObject);
	ThrowIfFailed(device.CreateBuffer(&constantBufferDesc, nullptr, &mConstantVertexBuffer), "ID3D11Device::CreateBuffer() failed.");
}

PolygonRenderer::~PolygonRenderer()
{
}

void PolygonRenderer::Draw()
{
	static float angle = 0.0f;

	angle += 0.001f;

	XMStoreFloat4x4(&mWorldMatrix, XMMatrixOrthographicRH(1024, 768, -1, 1) * XMMatrixRotationZ(angle));

	mContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	mContext->IASetInputLayout(mInputLayout);

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
	mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, offset);

	mContext->VSSetShader(mVertexShader, nullptr, 0);
	mContext->PSSetShader(mPixelShader, nullptr, 0);
	mContext->GSSetShader(mGeomShader, nullptr, 0);

	float width = 0.02f;

	mContext->UpdateSubresource(mConstantVertexBuffer, 0, nullptr, &mWorldMatrix, 0, 0);
	mContext->VSSetConstantBuffers(0, 1, &mConstantVertexBuffer);

	mContext->UpdateSubresource(mConstantGeometryBuffer, 0, nullptr, &width, 0, 0);
	mContext->GSSetConstantBuffers(0, 1, &mConstantGeometryBuffer);

	mContext->DrawIndexed(2 * 9, 0, 0);
}