#include "pch.h"
#include "D3DRenderBuffer.h"


namespace Library
{

	D3DRenderBuffer::D3DRenderBuffer(ID3D11Device1& device, ID3D11DeviceContext& context) :
		mDevice(&device), mContext(&context), mVertexBuffer(nullptr), mIndexBuffer(nullptr), mStride(0), mUseIndices(true)
	{
	}

	D3DRenderBuffer::~D3DRenderBuffer()
	{
	}

	void D3DRenderBuffer::Init(bool createIndicesBuffer)
	{
		mUseIndices = createIndicesBuffer;
	}

	void D3DRenderBuffer::SetData(float * data, std::uint32_t size, std::uint32_t stride, std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt)
	{
		D3D11_BUFFER_DESC vertexBufferDesc;
		ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
		vertexBufferDesc.ByteWidth = size + stride;
		vertexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		D3D11_SUBRESOURCE_DATA vertexSubResourceData;
		ZeroMemory(&vertexSubResourceData, sizeof(vertexSubResourceData));
		vertexSubResourceData.pSysMem = data;
		ThrowIfFailed(mDevice->CreateBuffer(&vertexBufferDesc, &vertexSubResourceData, &mVertexBuffer), "ID3D11Device::CreateBuffer() failed.");
		
		if (mUseIndices)
		{
			D3D11_BUFFER_DESC indexBufferDesc;
			ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
			indexBufferDesc.ByteWidth = indices_size;
			indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

			D3D11_SUBRESOURCE_DATA indexSubResourceData;
			ZeroMemory(&indexSubResourceData, sizeof(indexSubResourceData));
			indexSubResourceData.pSysMem = indices;
			ThrowIfFailed(mDevice->CreateBuffer(&indexBufferDesc, &indexSubResourceData, &mIndexBuffer), "ID3D11Device::CreateBuffer() failed.");

		}
		mStride = stride;
		UNREFERENCED_PARAMETER(elementCnt);
	}
	void D3DRenderBuffer::Use()
	{
		UINT offset = 0;
		mContext->IASetVertexBuffers(0, 1, &mVertexBuffer, &mStride, &offset);
		if(mUseIndices)
			mContext->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	}
}