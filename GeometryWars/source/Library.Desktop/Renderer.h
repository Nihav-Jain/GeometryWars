#pragma once

namespace Library {

	class RenderDevice;

	/**
	* The core render module, it knows all renderable objects and the device type
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

		void Update();

	private:
		Renderer(RenderDeviceType type);

		RenderDevice * mDevice;
		RenderDeviceType mDeviceType;
	};

}