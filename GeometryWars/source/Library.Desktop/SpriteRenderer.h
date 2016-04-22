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
	class SpriteRenderer : public ActionList, public Renderable
	{
		RTTI_DECLARATIONS(SpriteRenderer, ActionList);

	public:
		static const std::string ATTRIBUTE_TEXTURE2D;

		SpriteRenderer();
		virtual ~SpriteRenderer() = default;

		SpriteRenderer(const SpriteRenderer & rhs) = delete;
		SpriteRenderer & operator=(const SpriteRenderer & rhs) = delete;

		const std::string & GetImageTexture() const;
		void SetImageTexture(const glm::vec4 & color);

		virtual void Render(RenderDevice * device) override;

	private:
		void Init(RenderDevice * device);
		bool mInited;
		Texture * mTexture;
		Shader * mShader;
		RenderBuffer * mBuffer;

		Datum * mPosition;
		Datum * mRotation;
		Datum * mScale;
		glm::vec4 * mSize;
	};
}

