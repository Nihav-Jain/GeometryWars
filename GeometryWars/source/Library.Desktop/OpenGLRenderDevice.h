#pragma once
#include "RenderDevice.h"
#include "OpenGLShader.h"
#include <vector>

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

		void InitOpenGl();

		virtual Viewport * CreateViewport() override;

		virtual std::uint32_t CreateTexture(const std::string & imagePath) override;
		virtual void UseTexture(std::uint32_t) override;

		virtual Shader * CreateShader(const std::string & vPath, const std::string & fPath) override;
		virtual std::uint32_t CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride) override;
		virtual void UseBuffer(std::uint32_t) override;

		virtual void Draw() override;

	private:
		// The actuall draw call
		virtual void Invalid() override;

		GLFWwindow * mWindow;

		std::vector<OpenGLShader> mShaders;
	};

}

