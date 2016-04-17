#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"

namespace Library {
	/**
	* The renderable sprite class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	* TODO: Maybe drive from some renderable class?
	*/
	class Sprite : public Attributed
	{
		RTTI_DECLARATIONS(Sprite, Attributed);

	public:
		static const std::uint32_t NUM_RESERVED_PRESCRIBED_ATTRIBUTES;
		static const std::string ATTRIBUTE_POSITION;
		static const std::string ATTRIBUTE_IMAGE_PATH;

		Sprite();
		virtual ~Sprite() = default;

		Sprite(const Sprite & rhs) = delete;
		Sprite & operator=(const Sprite & rhs) = delete;

		const glm::vec4 & GetPosition() const;
		const std::string & GetImagePath() const;

	private:
		glm::vec4 mPosition;
		std::string mImagePath;
	};
}

