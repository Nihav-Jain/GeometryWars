#pragma once
#include "glm\vec4.hpp"
#include "RTTI.h"
#include "Attributed.h"
#include "Renderable.h"

namespace Library {

	class Texture;
	class Shader;
	class Buffer;

	/**
	* The renderable sprite class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	*/
	class SpriteRenderer : public ActionList, public Renderable
	{
		RTTI_DECLARATIONS(SpriteRenderer, ActionList);

	public:
		static const std::string ATTRIBUTE_TEXTURE2D;
		static const std::string ATTRIBUTE_LAYER;

		SpriteRenderer();
		virtual ~SpriteRenderer();

		SpriteRenderer(const SpriteRenderer & rhs) = default;
		SpriteRenderer & operator=(const SpriteRenderer & rhs) = delete;

		void SetTransform(const glm::vec4 & position, const glm::vec4 & rotation, const glm::vec4 & scale);
		void SetUV(float origin_x, float origin_y, float width, float height);

		virtual void Render(RenderDevice * device) override;
		virtual void BeginPlay(WorldState & worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;
		virtual Scope* Clone(const Scope& rhs) const override;


	private:
		void Init(RenderDevice * device);
		void SetBufferData();
		bool mInited;
		Texture * mTexture;
		Shader * mShader;
		Buffer * mBuffer;

		Datum * mPosition;
		Datum * mRotation;
		Datum * mScale;
		glm::vec4 * mSize;

		float mU;
		float mV;
		float mUWidth;
		float mVHeight;

		bool mIsStatic;
		glm::vec4 mStaticPosition;
		glm::vec4 mStaticRotation;
		glm::vec4 mStaticScale;

		std::int32_t mLayerNumber;
	};

	CONCRETE_ACTION_FACTORY(SpriteRenderer);
}

