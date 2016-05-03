#pragma once
#include <vector>

namespace Library {

	class Texture;
	/**
	* The FrameBuffer information
	*/
	class FrameBuffer
	{
	public:
		~FrameBuffer() = default;
		virtual void Init(std::uint32_t textureCnt, std::int32_t width, std::int32_t height) = 0;
		virtual void Use() = 0;

		virtual const std::vector<Texture *> & GetFrameTexture() = 0;
		virtual void ClearColor(glm::vec4 color) = 0;
	protected:
		FrameBuffer() = default;
	};

}

