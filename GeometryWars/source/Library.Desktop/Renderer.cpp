#include "pch.h"
#include "Renderer.h"
#include "RenderDevice.h"
#include "Renderable.h"
#include <algorithm>

namespace Library {

	Renderer * Renderer::sInstance = nullptr;

	Renderer::Renderer(RenderDevice * device) :
		mDevice(device)
	{
	}

	Renderer::~Renderer()
	{
		// TODO Clean up
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
		for (auto obj : mObjects) {
			obj->Render(mDevice);
		}

		if (mDevice != nullptr) {
			mDevice->Invalid();
		}
	}


}