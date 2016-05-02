#pragma once
namespace Library {

	class RenderBuffer
	{
	public:
		virtual ~RenderBuffer();

		virtual void Init(bool createIndicesBuffer) = 0;
		virtual void SetData(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt) = 0;
		virtual void Use() = 0;
	protected:
		RenderBuffer();
	};

}
