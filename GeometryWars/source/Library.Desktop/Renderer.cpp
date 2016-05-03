#include "pch.h"
#include "Renderer.h"
#include "RenderDevice.h"
#include "Renderable.h"
#include "PostProcessing.h"
#include "FrameBuffer.h"
#include "Shader.h"
#include "Buffer.h"
#include "Texture.h"
#include <algorithm>

namespace Library {

	Renderer * Renderer::sInstance = nullptr;

	Renderer::Renderer(RenderDevice * device) :
		mInited(false),
		mDevice(device),
		mDefaultFrameBuffer(nullptr),
		mShader(nullptr),
		mBuffer(nullptr)
	{
	}

	Renderer::~Renderer()
	{
		for (auto & it : mLayers) {
			if (it.second.TargetFrameBuffer != nullptr)
				delete it.second.TargetFrameBuffer;
			if (it.second.PostProcessingFrameBuffer != nullptr)
				delete it.second.PostProcessingFrameBuffer;
		}

		if (mShader != nullptr) {
			delete mShader;
		}
		if (mBuffer != nullptr) {
			delete mBuffer;
		}
	}

	void Renderer::AddPostPostProcessing(PostProcessing * postProcessing, std::uint32_t layerId)
	{
		if (mLayers.find(layerId) == mLayers.end()) {
			CreateNewLayer(layerId);
		}
		mLayers[layerId].PostProcessings.push_back(postProcessing);
	}

	void Renderer::AddRenderable(Renderable * object, std::uint32_t layerId)
	{
		if (mLayers.find(layerId) == mLayers.end()) {
			CreateNewLayer(layerId);
		}
		mLayers[layerId].Objects.push_back(object);
	}

	void Renderer::RemoveRenderable(Renderable * object, std::uint32_t layerId)
	{
		if (mLayers.find(layerId) != mLayers.end()) {
			auto itr = std::find(mLayers[layerId].Objects.begin(), mLayers[layerId].Objects.end(), object);
			if (itr != mLayers[layerId].Objects.end())
			{
				mLayers[layerId].Objects.erase(itr);
			}
		}
	}

	void Renderer::Update()
	{
		if (mDevice == nullptr)
			return;

		if (!mInited) {
			Init();
		}
		
		for (auto & it : mLayers) {

			if (it.second.TargetFrameBuffer == nullptr)
				it.second.TargetFrameBuffer = mDevice->CreateFrameBuffer(1);
			if (it.second.PostProcessingFrameBuffer == nullptr)
				it.second.PostProcessingFrameBuffer = mDevice->CreateFrameBuffer(1);

			if (it.second.TargetFrameBuffer != nullptr)
			{
				it.second.TargetFrameBuffer->Use();
				it.second.TargetFrameBuffer->ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			}
			// objects
			for (auto obj : it.second.Objects) {
				obj->Render(mDevice);
			}

			if (it.second.TargetFrameBuffer != nullptr) {
				FrameBuffer * buff = it.second.TargetFrameBuffer;
				// Post processing
				for (auto & p = it.second.PostProcessings.begin();
					p != it.second.PostProcessings.end(); ++p) {
					(*p)->Apply(mDevice, buff, it.second.PostProcessingFrameBuffer);
				}
			}
		}

		// Render to the final screen
		if (mDefaultFrameBuffer)
		{
			mDefaultFrameBuffer->Use();
			mDefaultFrameBuffer->ClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		}

		for (auto & it : mLayers) {
			mShader->Use();
			mBuffer->Use();

			FrameBuffer * buff = it.second.PostProcessings.size() == 0 ?
				it.second.TargetFrameBuffer : it.second.PostProcessingFrameBuffer;

			if(buff != nullptr)
				buff->GetFrameTexture()[0]->Use(0);

			mDevice->Draw();
		}

		mDevice->ClearScreen();
	}

	void Renderer::Init()
	{
		mInited = true;

		mDefaultFrameBuffer = mDevice->GetDefaultFrameBuffer();

		float vertices[] = {
			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,

			-1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 1.0f, 0.0f
		};

		mBuffer = mDevice->CreateBuffer(false);
		mShader = mDevice->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/sprite_f.glsl", "");

		if (mBuffer != nullptr)
			mBuffer->SetData(vertices, sizeof(vertices), 4 * sizeof(float), nullptr, 0, 4);
	}

	void Renderer::CreateNewLayer(std::uint32_t layerId)
	{
		Layer layer;
		layer.TargetFrameBuffer = nullptr;
		layer.PostProcessingFrameBuffer = nullptr;
		mLayers[layerId] = layer;
	}
}