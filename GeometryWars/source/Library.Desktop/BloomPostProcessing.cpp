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
		mShaderHDR(nullptr),
		mShaderBlur(nullptr),
		mBuffer(nullptr),
		mShaderBlend(nullptr)
	{
		mFrameBufferForBlur[0] = nullptr;
		mFrameBufferForBlur[1] = nullptr;

	}


	BloomPostProcessing::~BloomPostProcessing()
	{
		if (mFrameBufferForBlur[0] != nullptr)
			delete mFrameBufferForBlur[0];
		if (mFrameBufferForBlur[1] != nullptr)
			delete mFrameBufferForBlur[1];
		if (mFrameBuffer != nullptr)
			delete mFrameBuffer;
		if (mShaderHDR != nullptr)
			delete mShaderHDR;
		if (mShaderBlur != nullptr)
			delete mShaderBlur;
		if (mBuffer != nullptr)
			delete mBuffer;
	}

	void BloomPostProcessing::Apply(RenderDevice * device, FrameBuffer * src_buffer, FrameBuffer * target_buffer)
	{
		// Refered http://www.learnopengl.com/#!Advanced-Lighting/Bloom
		if (!mInited) {
			Init(device);
		}
		FrameBuffer * target = target_buffer;;

		// Find HDR
		mFrameBuffer->Use();
		mFrameBuffer->ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		src_buffer->GetFrameTexture()[0]->Use();
		mShaderHDR->Use();
		mBuffer->Use();
		device->Draw();

		// Blur
		bool horizontal = true;
		mFrameBufferForBlur[0]->Use();
		mFrameBufferForBlur[0]->ClearColor(glm::vec4(0.0f, .0f, 0.0f, 0.0f));
		mFrameBufferForBlur[1]->Use();
		mFrameBufferForBlur[1]->ClearColor(glm::vec4(0.0f, .0f, 0.0f, 0.0f));

		for (std::uint32_t i = 0; i < 10; i++) {

			mShaderBlur->Use();
			mBuffer->Use();

			mFrameBufferForBlur[horizontal]->Use();
			mShaderBlur->SetFloat("colorThreshold", 0.04f);
			mShaderBlur->SetBool("horizontal", horizontal);

			if (i == 0) {
				mFrameBuffer->GetFrameTexture()[1]->Use();
			}
			else {
				mFrameBufferForBlur[!horizontal]->GetFrameTexture()[0]->Use();
			}

			horizontal = !horizontal;

			device->Draw();
		}

		// Blend
		target->Use();
		target->ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

		mShaderBlend->Use();
		mShaderBlend->SetInt("scene", 0);
		mShaderBlend->SetInt("bloomBlur", 1);
		mShaderBlend->SetFloat("exposure", 3.0f);

		mFrameBuffer->GetFrameTexture()[0]->Use(0);
		mFrameBufferForBlur[0]->GetFrameTexture()[0]->Use(1);
		mBuffer->Use();

		device->Draw();
	}

	void BloomPostProcessing::Init(RenderDevice * device)
	{
		mInited = true;
		if (device == nullptr)
			return;

		mFrameBuffer = device->CreateFrameBuffer(2);
		mFrameBufferForBlur[0] = device->CreateFrameBuffer(1);
		mFrameBufferForBlur[1] = device->CreateFrameBuffer(1);
		mBuffer = device->CreateBuffer(false);
		mShaderHDR = device->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/bloom_f.glsl", "");
		mShaderBlur = device->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/bloom_blur_f.glsl", "");
		mShaderBlend = device->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/bloom_blend_f.glsl", "");

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
