#pragma once
namespace Library {

	class Viewport;

	/**
	* Render device object, for OpenGL and D3D implementation
	*/
	class RenderDevice
	{
		friend class Renderer;
	public:
		RenderDevice();
		virtual ~RenderDevice();

		virtual bool IsValid() = 0;

		virtual Viewport * CreateViewport() = 0;

	private:
		// The actual drawing call
		virtual void Draw() = 0;
		// TODO: more interface
	};

}