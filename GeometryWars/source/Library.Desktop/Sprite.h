#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"
#include "Renderable.h"

namespace Library {

	class Texture;
	class Shader;
	class RenderBuffer;

	/**
	* The renderable sprite class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	*/
	class Sprite : public ActionList, public Renderable
	{
		RTTI_DECLARATIONS(Sprite, ActionList);

	public:
		static const std::string ATTRIBUTE_TEXTURE2D;

		Sprite();
		virtual ~Sprite() = default;

		Sprite(const Sprite & rhs) = delete;
		Sprite & operator=(const Sprite & rhs) = delete;

		const std::string & GetImageTexture() const;
		void SetImageTexture(const glm::vec4 & color);

		virtual void Init(RenderDevice * device) override;
		virtual void Render(RenderDevice * device) override;

	private:
		Texture * mTexture;
		Shader * mShader;
		RenderBuffer * mBuffer;

		glm::vec4 * mPosition;
		glm::vec4 * mRotation;
		glm::vec4 * mScale;
		glm::vec4 * mSize;
	};
}

