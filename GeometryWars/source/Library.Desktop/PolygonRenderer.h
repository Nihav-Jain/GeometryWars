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
	* The renderable polygon class
	* TODO: Add trasnformation to the entiy trasnformation accordingly
	*/
	class PolygonRenderer : public ActionList, public Renderable
	{
		RTTI_DECLARATIONS(PolygonRenderer, ActionList);

	public:
		static const std::string ATTRIBUTE_INDICES;
		static const std::string ATTRIBUTE_POINTS;

		PolygonRenderer();
		virtual ~PolygonRenderer();

		PolygonRenderer(const PolygonRenderer & rhs);
		PolygonRenderer & operator=(const PolygonRenderer & rhs) = delete;

		virtual void Render(RenderDevice * device) override;
		virtual void BeginPlay(WorldState& worldState) override;
		virtual void OnDestroy(WorldState& worldState) override;

		virtual Scope* Clone(const Scope& rhs) const override;

	private:
		void Init(RenderDevice * device);
		bool mInited;
		Shader * mShader;
		Buffer * mBuffer;

		float mWidth;
		glm::vec4 mColor;

		Datum * mPosition;
		Datum * mRotation;
		Datum * mScale;
		Datum * mIndices;
		Datum * mPoints;
	};

	CONCRETE_ACTION_FACTORY(PolygonRenderer);
}

