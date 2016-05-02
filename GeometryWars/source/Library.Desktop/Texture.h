#pragma once

namespace Library {

	class RenderDevice;

	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual void Use() = 0;
		virtual void Init(const std::string & imagePath) = 0;
	protected:
		Texture() = default;
	};

}
