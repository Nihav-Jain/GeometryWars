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
		Renderer(RenderDevice * device);
		~Renderer();

		Renderer(const Renderer &) = delete;
		Renderer & operator=(const Renderer &) = delete;

		// TODO: Should the factory create the object from the XML accordingly or should we hava a way for the renderer create the obj 
		void AddRenderable(Renderable *);

		void Update();

	private:
		RenderDevice * mDevice;
		std::vector<Renderable*> mObjects;
	};

}