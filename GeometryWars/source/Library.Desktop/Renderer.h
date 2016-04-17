#pragma once
#include <vector>

namespace Library {

	class RenderDevice;
	class Renderable;

	/**
	* The core render module
	*/
	class Renderer
	{
	public:

		enum class RenderDeviceType
		{
			OPENGL,
			D3D
		};

		static Renderer * CreateRenderer(RenderDeviceType type);

		~Renderer();

		Renderer(const Renderer &) = delete;
		Renderer & operator=(const Renderer &) = delete;

		// TODO: Should the factory create the object from the XML accordingly or should we hava a way for the renderer create the obj 
		void AddRenderable(Renderable *);

		void Update();

	private:
		Renderer(RenderDeviceType type);

		RenderDevice * mDevice;
		RenderDeviceType mDeviceType;
		std::vector<Renderable*> mObjects;
	};

}