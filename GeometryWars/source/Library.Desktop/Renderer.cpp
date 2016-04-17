#include "pch.h"
#include "Renderer.h"
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
		mObjects.push_back(object);
	}

	void Renderer::Update()
	{
		for (auto & obj : mObjects) {
			obj->Render(mDevice);
		}
	}


}