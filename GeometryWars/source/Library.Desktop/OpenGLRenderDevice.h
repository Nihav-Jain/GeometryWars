#pragma once
#include "RenderDevice.h"

class GLFWwindow;

namespace Library {

	class OpenGLRenderDevice final :
		public RenderDevice
	{
	public:
		OpenGLRenderDevice();
		virtual ~OpenGLRenderDevice();

		OpenGLRenderDevice(const OpenGLRenderDevice &) = delete;
		OpenGLRenderDevice & operator=(const OpenGLRenderDevice &) = delete;

		virtual bool IsValid() override;

		void InitOpenGl();

		virtual Viewport * CreateViewport() override;

	private:
		// The actuall draw call
		virtual void Draw() override;

		GLFWwindow * mWindow;
	};

}

