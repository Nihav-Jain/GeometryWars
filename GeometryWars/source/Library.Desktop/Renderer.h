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
		// TODO: Remove this !!!!!!!!
		static Renderer * GetInstance(RenderDevice * device = nullptr) {
			if (sInstance == nullptr)
				sInstance = new Renderer(device);
			return  sInstance;
		};


		Renderer(RenderDevice * device);
		~Renderer();

		Renderer(const Renderer &) = delete;
		Renderer & operator=(const Renderer &) = delete;

		// TODO: Should the factory create the object from the XML accordingly or should we hava a way for the renderer create the obj 
		void AddRenderable(Renderable *);
		void RemoveRenderable(Renderable *);

		void Update();

	private:
		// TODO: Remove this !!!!!!!!
		static Renderer * sInstance;


		RenderDevice * mDevice;
		std::vector<Renderable*> mObjects;
	};

}