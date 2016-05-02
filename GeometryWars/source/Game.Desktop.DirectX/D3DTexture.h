#pragma once
#include <Texture.h>
namespace Library
{
	class D3DTexture : public Library::Texture
	{
	public:
		D3DTexture();
		~D3DTexture();

		virtual void Init(const std::string & imagePath) override;
		virtual void Use() override;
	};
}