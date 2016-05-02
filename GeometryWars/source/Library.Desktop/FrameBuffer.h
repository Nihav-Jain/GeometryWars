#pragma once


namespace Library {

	class Texture;
	/**
	* The FrameBuffer information
	*/
	class FrameBuffer
	{
	public:
		~FrameBuffer() = default;
		virtual void Init(std::int32_t width, std::int32_t height) = 0;
		virtual void Use() = 0;

		virtual Texture * GetFrameTexture() = 0;
	protected:
		FrameBuffer() = default;
	};

}

