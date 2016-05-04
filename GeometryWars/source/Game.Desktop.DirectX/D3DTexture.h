#pragma once
#include <Texture.h>
namespace Library
{
	class D3DTexture : public Library::Texture
	{
	public:
		D3DTexture(ID3D11Device1& device, ID3D11DeviceContext& context);
		~D3DTexture();

		virtual void Init(const std::string & imagePath) override;
		virtual void Use(std::uint32_t useAsTextureIndex = 0) override;
	private:
		ID3D11Device1* mDevice;
		ID3D11DeviceContext* mContext;
		ID3D11ShaderResourceView* mColorTexture;
		ID3D11SamplerState* mColorSampler;
	};
}