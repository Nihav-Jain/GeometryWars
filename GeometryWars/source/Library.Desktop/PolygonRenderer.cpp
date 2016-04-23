#include "pch.h"
#include "PolygonRenderer.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "Image.h"
#include "RenderBuffer.h"
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace Library {

	RTTI_DEFINITIONS(PolygonRenderer);

	const std::string PolygonRenderer::ATTRIBUTE_INDICES = "indices";
	const std::string PolygonRenderer::ATTRIBUTE_POINTS = "points";

	PolygonRenderer::PolygonRenderer() :
		mInited(false),
		mShader(nullptr),
		mBuffer(nullptr),
		mPosition(nullptr),
		mRotation(nullptr),
		mScale(nullptr),
		mIndices(nullptr),
		mPoints(nullptr)
	{
		AddExternalAttribute("color", 1, &mColor);
	}

	void PolygonRenderer::Render(RenderDevice * device)
	{
		if (!mInited) {
			Init(device);
		}

		if (device == nullptr)
			return;
		mShader->Use();

		glm::mat4 model;

		glm::vec4 pos = mPosition->Get<glm::vec4>();
		glm::vec4 rotate = mRotation->Get<glm::vec4>();
		glm::vec4 scale = mScale->Get<glm::vec4>();

		// TODO: Handle nullptr case
		model = glm::translate(model, glm::vec3(pos));
		model = glm::rotate(model, rotate.x, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::scale(model, glm::vec3((*mSize).x, (*mSize).y, 1.0f));
		model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));

		// TODO: Get Viewport size
		glm::mat4 projection = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f);
		mShader->SetMatrix4("model_view_projection", projection * model);

		mShader->SetVector4("inputColor", mColor);

		mBuffer->Use();
		device->Draw(RenderDevice::DrawMode::LINES, mIndices->Size());
	}

	void PolygonRenderer::Init(RenderDevice * device)
	{
		mInited = true;
		if (device == nullptr)
			return;

		// TODO: Handle failed case
		mPosition = Search("position");
		mRotation = Search("rotation");
		mScale = Search("scale");

		mIndices = Search("indices");
		mPoints = Search("points");

		assert(mIndices != nullptr);
		assert(mPoints != nullptr);

		mShader = device->CreateShader("Content/shader/glsl/polygon_v.glsl", "Content/shader/glsl/polygon_f.glsl");

		std::uint32_t cnt = mPoints->Size();
		std::uint32_t indicesCnt = mIndices->Size();
		float vertices[256] = { 0 };
		std::uint32_t indices[1024] = { 0 };

		for (std::uint32_t i = 0, j = 0; i < cnt; ++i, ++j) {
			vertices[j] = mPoints->Get<glm::vec4>(i).x;
			vertices[++j] = mPoints->Get<glm::vec4>(i).y;
		}

		for (std::uint32_t i = 0; i < indicesCnt; ++i) {
			indices[i] = (std::uint32_t)mIndices->Get<std::int32_t>(i);
		}

		mBuffer = device->CreateBuffer(vertices, cnt * 2 * sizeof(float), 2 * sizeof(float), indices, indicesCnt * sizeof(float), 2);
	}

}
