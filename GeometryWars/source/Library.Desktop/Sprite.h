#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"
#include "Renderable.h"

namespace Library {

	class Shader;

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
		static const std::string ATTRIBUTE_COLOR;

		Sprite();
		virtual ~Sprite() = default;

		Sprite(const Sprite & rhs) = delete;
		Sprite & operator=(const Sprite & rhs) = delete;

		const glm::vec4 & GetPosition() const;
		const std::string & GetImagePath() const;
		const glm::vec4 & GetColor() const;

		void SetPosition(const glm::vec4 & position);
		void SetImagePath(const std::string & imagePath);
		void SetColor(const glm::vec4 & color);

		virtual void Init(RenderDevice * device) override;
		virtual void Render(RenderDevice * device) override;

	private:
		glm::vec4 mPosition;
		std::string mImagePath;
		glm::vec4 mColor;

		std::uint32_t mTextureId;
		Shader * mShader;
		std::uint32_t mBufferId;
	};
}

