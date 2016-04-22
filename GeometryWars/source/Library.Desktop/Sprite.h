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

