#include "pch.h"
#include "D3DRenderBuffer.h"


namespace Library
{

	D3DRenderBuffer::D3DRenderBuffer()
	{
	}


	D3DRenderBuffer::~D3DRenderBuffer()
	{
	}
	void D3DRenderBuffer::Init(float * data, std::uint32_t size, std::uint32_t stride, std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt)
	{
		UNREFERENCED_PARAMETER(data);
		UNREFERENCED_PARAMETER(size);
		UNREFERENCED_PARAMETER(stride);
		UNREFERENCED_PARAMETER(indices);
		UNREFERENCED_PARAMETER(indices_size);
		UNREFERENCED_PARAMETER(elementCnt);
	}
	void D3DRenderBuffer::Use()
	{
	}
}