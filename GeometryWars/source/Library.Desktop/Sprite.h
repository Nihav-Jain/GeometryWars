#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"
#include "Renderable.h"

namespace Library {
	/**
	* The renderable sprite class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	*/
	class Sprite : public Attributed, public Renderable
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

		virtual void Render(RenderDevice * device) override;

	private:
		glm::vec4 mPosition;
		std::string mImagePath;
	};
}

