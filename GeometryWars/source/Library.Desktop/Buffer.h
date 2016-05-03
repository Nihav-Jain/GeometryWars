#pragma once
namespace Library {

	class Buffer
	{
	public:
		virtual ~Buffer() = default;

		virtual void Init(bool createIndicesBuffer) = 0;
		virtual void SetData(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt) = 0;
		virtual void Use() = 0;
	protected:
		Buffer() = default;
	};

}
