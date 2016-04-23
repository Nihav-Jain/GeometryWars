#include "pch.h"
#include "Renderer.h"
#include "RenderDevice.h"
#include "Renderable.h"

namespace Library {

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
		object->Init(mDevice);
		mObjects.push_back(object);
	}

	void Renderer::Update()
	{
		for (auto & obj : mObjects) {
			obj->Render(mDevice);
		}

		if (mDevice != nullptr) {
			mDevice->Invalid();
		}
	}


}