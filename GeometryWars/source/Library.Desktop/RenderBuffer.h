#pragma once
namespace Library {

	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer();

		virtual void Init(float * data, std::uint32_t size, std::uint32_t stride) = 0;
		virtual void Use() = 0;
	protected:
		RenderBuffer();
	};

}
