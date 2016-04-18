#pragma once


namespace Library {

	class RenderDevice;

	/**
	* Renderable object interface
	*/
	class Renderable
	{
		friend class Renderer;

	public:
		Renderable() = default;
		virtual ~Renderable() = default;

		virtual void Render(RenderDevice * device) = 0;
	};
}

