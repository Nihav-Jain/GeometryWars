#pragma once
#include <RenderBuffer.h>

namespace Library
{

	class D3DRenderBuffer : public Library::RenderBuffer
	{
	public:
		D3DRenderBuffer();
		virtual ~D3DRenderBuffer();

		virtual void Init(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt);

		virtual void Use();
	private:
		struct VertexPositionColor
		{
			XMFLOAT4 Position;
			XMFLOAT4 Color;
		};
		struct CVertexBufferPerObject
		{
			XMFLOAT4X4 WorldViewProjection;
		};
	};
}
