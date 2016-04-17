#include "pch.h"
#include "Renderer.h"
#include "Renderable.h"

namespace Library {

	Renderer::Renderer(RenderDeviceType type) :
		mDeviceType(type)
	{
	}

	Renderer::~Renderer()
	{
		// TODO Clean up
	}

	Renderer * Renderer::CreateRenderer(RenderDeviceType type)
	{
		Renderer * renderer = nullptr;
		if (type == RenderDeviceType::OPENGL) {
			renderer = new Renderer(type);
		}
		else {
			throw std::exception("No implementation");
		}

		return renderer;
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