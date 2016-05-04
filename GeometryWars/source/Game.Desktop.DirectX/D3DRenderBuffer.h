#pragma once
#include <Buffer.h>

struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
namespace Library
{
	class D3DRenderBuffer : public Buffer
	{
	public:
		D3DRenderBuffer(ID3D11Device1& device, ID3D11DeviceContext& context);
		virtual ~D3DRenderBuffer();

		virtual void Init(bool createIndicesBuffer) override;
		virtual void SetData(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt) override;

		virtual void Use() override;
	private:
		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11Buffer* mVertexBuffer;
		ID3D11Buffer* mIndexBuffer;
		bool mUseIndices;
		UINT mStride;
	};
}
