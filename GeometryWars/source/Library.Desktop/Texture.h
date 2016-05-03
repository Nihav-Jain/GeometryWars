#pragma once

namespace Library {

	class RenderDevice;

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Use(std::uint32_t useAsTextureIndex = 0) = 0;
		virtual void Init(const std::string & imagePath) = 0;
	protected:
		Texture() = default;
	};

}
