#include "pch.h"
#include "PolygonRenderer.h"
#include "GameException.h"
#include <vector>
#include "Utility.h"

using namespace Library;
PolygonRenderer::PolygonRenderer(ID3D11Device1 & device, ID3D11DeviceContext & context): m_device(&device), m_context(&context), m_pixel_shader(nullptr), m_vertex_shader(nullptr), m_vertex_buffer(m_vertex_buffer),
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
	ThrowIfFailed(device.CreateVertexShader(&compiledVertexShader[0], compiledVertexShader.size(), nullptr, &m_vertex_shader), "ID3D11Device::CreatedVertexShader() failed.");

	// Load a compiled pixel shader
	std::vector<char> compiledPixelShader;
	Utility::LoadBinaryFile(L"PixelShader.cso", compiledPixelShader);
	ThrowIfFailed(device.CreatePixelShader(&compiledPixelShader[0], compiledPixelShader.size(), nullptr, &m_pixel_shader), "ID3D11Device::CreatedPixelShader() failed.");

	// Create an input layout
	D3D11_INPUT_ELEMENT_DESC inputElementDescriptions[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	ThrowIfFailed(device.CreateInputLayout(inputElementDescriptions, ARRAYSIZE(inputElementDescriptions), &compiledVertexShader[0], compiledVertexShader.size(), &m_input_layout), "ID3D11Device::CreateInputLayout() failed.");

	// Create a vertex buffer
	VertexPositionColor vertices[] =
	{
		VertexPositionColor { XMFLOAT4(-1.0f,-0.5f, 0.5f, 1.f), XMFLOAT4(1.0f, 0, 0, 0)},
		VertexPositionColor { XMFLOAT4(-0.5f, 0.5f, 0.5f, 1.f), XMFLOAT4(0, 1.0f, 0, 0)},
		VertexPositionColor { XMFLOAT4(0.0f,-0.5f, 0.5f, 1.f), XMFLOAT4(0, 0, 1.0f, 0)},
		VertexPositionColor { XMFLOAT4(0.0f,-0.5f, 0.5f, 1.f), XMFLOAT4(1.0f, 0, 0, 0)},
		VertexPositionColor { XMFLOAT4(0.5f, 0.5f, 0.5f, 1.f), XMFLOAT4(0, 1.0f, 0, 0)},
		VertexPositionColor { XMFLOAT4(1.0f,-0.5f, 0.5f, 1.f), XMFLOAT4(0, 0, 1.0f, 0)}
	};

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = sizeof(VertexPositionColor) * ARRAYSIZE(vertices);
	vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexSubResourceData;
	ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
	vertexSubResourceData.pSysMem = vertices;
	ThrowIfFailed(device.CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &m_vertex_buffer), "ID3D11Device::CreateBuffer() failed.");

	// Create a constant buffer
	D3D11_BUFFER_DESC constantBufferDesc;
	ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
	constantBufferDesc.ByteWidth = sizeof(CBufferPerObject);
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ThrowIfFailed(device.CreateBuffer(&constantBufferDesc, nullptr, &mConstantBuffer), "ID3D11Device::CreateBuffer() failed.");
}

PolygonRenderer::~PolygonRenderer()
{
}

void PolygonRenderer::Draw()
{
	static float angle = 0.0f;

	angle += 0.001f;

	XMStoreFloat4x4(&mWorldMatrix, XMMatrixRotationZ(angle));

	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_context->IASetInputLayout(m_input_layout);

	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	m_context->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	m_context->VSSetShader(m_vertex_shader, nullptr, 0);
	m_context->PSSetShader(m_pixel_shader, nullptr, 0);

	m_context->UpdateSubresource(mConstantBuffer, 0, nullptr, &mWorldMatrix, 0, 0);
	m_context->VSSetConstantBuffers(0, 1, &mConstantBuffer);

	m_context->Draw(6, 0);
}
