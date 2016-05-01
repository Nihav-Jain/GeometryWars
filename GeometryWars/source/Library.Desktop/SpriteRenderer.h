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
		virtual ~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer & rhs) = delete;
		SpriteRenderer & operator=(const SpriteRenderer & rhs) = delete;

		void SetUV(float origin_x, float origin_y, float width, float height);

		virtual void Render(RenderDevice * device) override;

	private:
		void Init(RenderDevice * device);
		void SetBufferData();
		bool mInited;
		Texture * mTexture;
		Shader * mShader;
		RenderBuffer * mBuffer;

		Datum * mPosition;
		Datum * mRotation;
		Datum * mScale;
		glm::vec4 * mSize;

		float mU;
		float mV;
		float mUWidth;
		float mVHeight;
	};
}

