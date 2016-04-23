#include "pch.h"
#include "Sprite.h"
#include "RenderDevice.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderBuffer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Library {

	RTTI_DEFINITIONS(Sprite, Attributed);

	const std::uint32_t Sprite::NUM_RESERVED_PRESCRIBED_ATTRIBUTES = 2;
	const std::string Sprite::ATTRIBUTE_POSITION = "position";
	const std::string Sprite::ATTRIBUTE_IMAGE_PATH = "imagePath";
	const std::string Sprite::ATTRIBUTE_COLOR = "color";

	Sprite::Sprite() :
		mTexture(nullptr),
		mShader(nullptr),
		mBuffer(nullptr)
	{
		AddExternalAttribute(ATTRIBUTE_POSITION, 1, &mPosition);
		AddExternalAttribute(ATTRIBUTE_IMAGE_PATH, 1, &mImagePath);
		AddExternalAttribute(ATTRIBUTE_COLOR, 1, &mColor);
	}

	const glm::vec4 & Sprite::GetPosition() const
	{
		return mPosition;
	}

	const std::string & Sprite::GetImagePath() const
	{
		return mImagePath;
	}

	const glm::vec4 & Sprite::GetColor() const
	{
		return mColor;
	}

	void Sprite::SetPosition(const glm::vec4 & position)
	{
		mPosition = position;
	}

	void Sprite::SetImagePath(const std::string & imagePath)
	{
		mImagePath = imagePath;
	}

	void Sprite::SetColor(const glm::vec4 & color)
	{
		mColor = color;
	}

	void Sprite::Render(RenderDevice * device)
	{
		if (device == nullptr)
			return;
		mShader->Use();

		glm::vec2 size(300, 400);
		float rotate = 45.0f;

		glm::mat4 model;
		model = glm::translate(model, glm::vec3(mPosition)); 

		model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
		model = glm::rotate(model, rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

		model = glm::scale(model, glm::vec3(size, 1.0f));
		glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
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

		mTexture = device->CreateTexture(mImagePath);
		mShader = device->CreateShader("Content/shader/glsl/sprite_v.glsl", "Content/shader/glsl/sprite_f.glsl");

		float vertices[] = {
			// Pos      // Tex
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