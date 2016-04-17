#pragma once
namespace Library {

	class Viewport;

	/**
	* Render device object, for OpenGL and D3D implementation
	*/
	class RenderDevice
	{
	public:
		RenderDevice();
		virtual ~RenderDevice();

		virtual Viewport * CreateViewport() = 0;

		// TODO: more interface
	};

}