#include "pch.h"
#include "Renderer.h"
#include "RenderDevice.h"
#include "Renderable.h"
#include "PostProcessing.h"
#include "FrameBuffer.h"
#include <algorithm>

namespace Library {

	Renderer * Renderer::sInstance = nullptr;

	Renderer::Renderer(RenderDevice * device) :
		mDevice(device),
		mFrameBuffer(nullptr),
		mDefaultFrameBuffer(nullptr)
	{
	}

	Renderer::~Renderer()
	{
		if (mFrameBuffer != nullptr) {
			delete mFrameBuffer;
		}
	}

	void Renderer::AddPostPostProcessing(PostProcessing * postProcessing)
	{
		mPostProcessings.push_back(postProcessing);
	}

	void Renderer::AddRenderable(Renderable * object)
	{
		mObjects.push_back(object);
	}

	void Renderer::RemoveRenderable(Renderable * object)
	{
		auto itr = std::find(mObjects.begin(), mObjects.end(), object);
		if(itr != mObjects.end())
		{
			mObjects.erase(itr);
		}
	}

	void Renderer::Update()
	{
		if (mFrameBuffer == nullptr) {
			mFrameBuffer = mDevice->CreateFrameBuffer(1);
		}

		if (mDefaultFrameBuffer == nullptr) {
			mDefaultFrameBuffer = mDevice->GetDefaultFrameBuffer();
		}

		mFrameBuffer->Use();
		mFrameBuffer->ClearColor(glm::vec4(0.0f, .0f, .0f, 1.0f));

		for (auto obj : mObjects) {
			obj->Render(mDevice);
		}

		FrameBuffer * buff = mFrameBuffer;
		for (auto & it = mPostProcessings.begin();
			it != mPostProcessings.end(); ++it) {
			
			if (it + 1 == mPostProcessings.end()) {
				buff = (*it)->Apply(mDevice, buff, mDefaultFrameBuffer);
			}
			else {
				buff = (*it)->Apply(mDevice, buff, nullptr);
			}
		}

		if (mDevice != nullptr) {
			mDevice->Invalid();
		}
	}

	void Renderer::RenderToScreen()
	{
	}


}