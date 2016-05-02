#pragma once
#include <RenderBuffer.h>

struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
namespace Library
{
	class D3DRenderBuffer : public Library::RenderBuffer
	{
	public:
		D3DRenderBuffer(ID3D11Device1& device, ID3D11DeviceContext& context);
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

		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		UINT mStride;
	};
}
