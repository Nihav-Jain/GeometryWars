#include "pch.h"
#include "SpriteRenderer.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "Image.h"
#include "Buffer.h"
#include "Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <math.h>

namespace Library {

	RTTI_DEFINITIONS(SpriteRenderer, ActionList);

	const std::string SpriteRenderer::ATTRIBUTE_TEXTURE2D = "texture2d";
	const std::string SpriteRenderer::ATTRIBUTE_LAYER = "layer";

	SpriteRenderer::SpriteRenderer() :
		mInited(false),
		mTexture(nullptr),
		mShader(nullptr),
		mBuffer(nullptr),
		mPosition(nullptr),
		mRotation(nullptr),
		mScale(nullptr),
		mSize(nullptr),
		mU(.0f),
		mV(.0f),
		mUWidth(1.0f),
		mVHeight(1.0f),

		mIsStatic(false),
		mStaticPosition(),
		mStaticRotation(),
		mStaticScale(),
		mLayerNumber(100)
	{
		AddExternalAttribute(ATTRIBUTE_LAYER, 1, &mLayerNumber);
	}

	SpriteRenderer::~SpriteRenderer()
	{
		if (mShader != nullptr) {
			delete mShader;
			mShader = nullptr;
		}
		if (mBuffer != nullptr) {
			delete mBuffer;
			mBuffer = nullptr;
		}
		if (mTexture != nullptr) {
			delete mTexture;
			mTexture = nullptr;
		}
	}

	void SpriteRenderer::SetTransform(const glm::vec4 & position, const glm::vec4 & rotation, const glm::vec4 & scale)
	{
		mIsStatic = true;
		mStaticPosition = position;
		mStaticRotation = rotation;
		mStaticScale = scale;
	}

	void SpriteRenderer::SetUV(float origin_x, float origin_y, float width, float height)
	{
		(origin_x); (origin_y); (width); (height);
		Image * image = FindAction("image")->As<Image>();
		assert(image != nullptr);

		glm::vec4 size = (*image)["size"].Get<glm::vec4>();

		mU = origin_x / size.x;
		mV = 1.0f - (origin_y + height) / size.y;
		mUWidth = width / size.x;
		mVHeight = height / size.y;

		SetBufferData();
	}

	void SpriteRenderer::Render(RenderDevice * device)
	{
		if (!mInited) {
			Init(device);
		}

		if (device == nullptr)
			return;
		mShader->Use();

		glm::mat4 model;


		// Dynamic image
		if (!mIsStatic)
		{
			glm::vec4 pos = mPosition->Get<glm::vec4>();
			glm::vec4 rotate = mRotation->Get<glm::vec4>();
			glm::vec4 scale = mScale->Get<glm::vec4>();

			// TODO: Handle nullptr case
			model = glm::translate(model, glm::vec3(pos));
			model = glm::rotate(model, rotate.x, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3((*mSize).x * mUWidth, (*mSize).y * mVHeight, 1.0f));
			model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f));
		}

		// Image does not change
		else
		{
			model = glm::translate(model, glm::vec3(mStaticPosition));
			model = glm::rotate(model, mStaticRotation.x, glm::vec3(0.0f, 0.0f, 1.0f));
			model = glm::scale(model, glm::vec3((*mSize).x * mUWidth, (*mSize).y * mVHeight, 1.0f));
			model = glm::scale(model, glm::vec3(mStaticScale.x, mStaticScale.y, mStaticScale.z));
		}

		float halfwidth = (float)device->GetViewportWidth() / 2;
		float halfheight = (float)device->GetViewportHeight() / 2;

		glm::mat4 projection = glm::ortho(-halfwidth, halfwidth, -halfheight, halfheight, -1.0f, 1.0f);
		mShader->SetMatrix4("model_view_projection", projection * model);
		
		mTexture->Use();
		mBuffer->Use();
		device->Draw();
	}

	Scope * SpriteRenderer::Clone(const Scope & rhs) const
	{
		SpriteRenderer& action = *rhs.AssertiveAs<SpriteRenderer>();
		return new SpriteRenderer(action);
	}

	void SpriteRenderer::Init(RenderDevice * device)
	{
		mInited = true;
		if (device == nullptr)
			return;

		Image * image = FindAction("image")->As<Image>();
		assert(image != nullptr);

		image->Init(device);
		image->GetImageInfo(&mTexture, &mSize);

		assert(mTexture != nullptr);

		// TODO: Handle failed case -- do not throw exception because I'm not using these! -Will
		mPosition = Search("position");
		mRotation = Search("rotation");
		mScale = Search("scale");

		mShader = device->CreateShader("Content/shader/glsl/sprite_v.glsl", "Content/shader/glsl/sprite_f.glsl", "");


		mBuffer = device->CreateBuffer(false);
		SetBufferData();
	}

	void SpriteRenderer::SetBufferData()
	{
		float vertices[] = {
			-0.5f, 0.5f, mU, mV + mVHeight,
			0.5f, -0.5f, mU + mUWidth, mV,
			-0.5f, -0.5f, mU, mV,

			-0.5f, 0.5f, mU, mV + mVHeight,
			0.5f, 0.5f, mU + mUWidth, mV + mVHeight,
			0.5f, -0.5f, mU + mUWidth, mV
		};
		if (mBuffer != nullptr)
			mBuffer->SetData(vertices, sizeof(vertices), 4 * sizeof(float), nullptr, 0, 4);
	}

	void SpriteRenderer::BeginPlay(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Renderer::GetInstance()->AddRenderable(this, mLayerNumber);

		if (mShader != nullptr) {
			delete mShader;
			mShader = nullptr;
		}
		if (mBuffer != nullptr) {
			delete mBuffer;
			mBuffer = nullptr;
		}
		if (mTexture != nullptr) {
			delete mTexture;
			mTexture = nullptr;
		}
	}

	void SpriteRenderer::OnDestroy(WorldState & worldState)
	{
		UNREFERENCED_PARAMETER(worldState);
		Renderer::GetInstance()->RemoveRenderable(this, mLayerNumber);
	}

}
