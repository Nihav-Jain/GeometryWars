#include "pch.h"
#include "Renderer.h"

namespace Library {

	Renderer::Renderer(RenderDeviceType type) :
		mDeviceType(type)
	{
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

	Renderer::~Renderer()
	{
	}

}