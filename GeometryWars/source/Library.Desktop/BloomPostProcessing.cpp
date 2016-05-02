#include "pch.h"
#include "BloomPostProcessing.h"
#include "RenderDevice.h"
#include "FrameBuffer.h"

namespace Library {

	BloomPostProcessing::BloomPostProcessing() :
		mInited(false),
		mFrameBuffer(nullptr),
		mTexture(nullptr),
		mShader(nullptr),
		mBuffer(nullptr)
	{
	}


	BloomPostProcessing::~BloomPostProcessing()
	{
		if (mFrameBuffer != nullptr)
			delete mFrameBuffer;
	}

	void BloomPostProcessing::Apply(RenderDevice * device, FrameBuffer * buffer)
	{
		if (!mInited) {
			Init(device);
		}
		(buffer);
		//mFrameBuffer->Use();
	}

	void BloomPostProcessing::Init(RenderDevice * device)
	{
		mInited = true;
		if (device == nullptr)
			return;

		mFrameBuffer = device->CreateFrameBuffer();
		mTexture = mFrameBuffer->GetFrameTexture();
		mShader = device->CreateShader("Content/shader/glsl/bloom_v.glsl", "Content/shader/glsl/bloom_f.glsl", "");
	}

}
