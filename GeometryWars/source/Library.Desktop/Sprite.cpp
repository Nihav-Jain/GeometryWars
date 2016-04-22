#include "pch.h"
#include "Sprite.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "Image.h"
#include "RenderBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Library {

	RTTI_DEFINITIONS(Sprite);


	const std::uint32_t Sprite::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 1;
	const std::string Sprite::ATTRIBUTE_TEXTURE2D = "texture2d";

	Sprite::Sprite() :
		mTexture(nullptr),
		mShader(nullptr),
		mBuffer(nullptr),
		mPosition(nullptr),
		mRotation(nullptr),
		mScale(nullptr),
		mSize(nullptr)
	{
	}

	void Sprite::Render(RenderDevice * device)
	{
		if (device == nullptr)
			return;
		mShader->Use();

		glm::vec2 size(100, 100);

		glm::mat4 model;

		// TODO: Handle nullptr case
		model = glm::translate(model, glm::vec3(*mPosition)); 
		model = glm::rotate(model, mPosition->x, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3((*mSize).x, (*mSize).y, 1.0f));
		model = glm::scale(model, glm::vec3((*mScale).x, (*mScale).y, 1.0f));

		// TODO: Get Viewport size
		glm::mat4 projection = glm::ortho(-400.0f, 400.0f, -300.0f, 300.0f, -1.0f, 1.0f);
		mShader->SetMatrix4("projection", projection);
		mShader->SetMatrix4("model", model);
		mTexture->Use();
		mBuffer->Use();
		device->Draw();
	}

	void Sprite::Init(RenderDevice * device)
	{
		if (device == nullptr)
			return;

		Image * image = FindAction("image")->As<Image>();
		assert(image != nullptr);

		image->Init(device);
		image->GetImageInfo(&mTexture, &mSize);

		assert(mTexture != nullptr);

		// TODO: Handle failed case
		mPosition = &Search("position")->Get<glm::vec4>();
		mRotation = &Search("rotation")->Get<glm::vec4>();
		mScale = &Search("scale")->Get<glm::vec4>();
		
		mShader = device->CreateShader("Content/shader/glsl/sprite_v.glsl", "Content/shader/glsl/sprite_f.glsl");

		float vertices[] = {
			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,

			0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 1.0f, 0.0f
		};

		mBuffer = device->CreateBuffer(vertices, sizeof(vertices), 4 * sizeof(float));
	}

}