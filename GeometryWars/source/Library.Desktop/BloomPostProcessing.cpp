#include "pch.h"
#include "BloomPostProcessing.h"
#include "RenderDevice.h"
#include "FrameBuffer.h"
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

	BloomPostProcessing::BloomPostProcessing() :
		mInited(false),
		mFrameBuffer(nullptr),
		mShader(nullptr),
		mBuffer(nullptr)
	{
	}


	BloomPostProcessing::~BloomPostProcessing()
	{
		if (mFrameBuffer != nullptr)
			delete mFrameBuffer;
		if (mShader != nullptr)
			delete mShader;
		if (mBuffer != nullptr)
			delete mBuffer;
	}

	FrameBuffer * BloomPostProcessing::Apply(RenderDevice * device, FrameBuffer * src_buffer, FrameBuffer * target_buffer)
	{
		if (!mInited) {
			Init(device);
		}
		FrameBuffer * target = target_buffer == nullptr ? mFrameBuffer : target_buffer;
		target->Use();
		target->ClearColor(glm::vec4(0.0f, .0f, .0f, 1.0f));

		src_buffer->GetFrameTexture()->Use();
		mShader->Use();
		mBuffer->Use();

		device->Draw();
		return mFrameBuffer;
	}

	void BloomPostProcessing::Init(RenderDevice * device)
	{
		mInited = true;
		if (device == nullptr)
			return;

		mFrameBuffer = device->CreateFrameBuffer();
		mBuffer = device->CreateBuffer(false);
		mShader = device->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/bloom_f.glsl", "");

		float vertices[] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};
		if (mBuffer != nullptr)
			mBuffer->SetData(vertices, sizeof(vertices), 4 * sizeof(float), nullptr, 0, 4);
	}

}
