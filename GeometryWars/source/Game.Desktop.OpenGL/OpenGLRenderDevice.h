#pragma once
#include "RenderDevice.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLRenderBuffer.h"
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
		virtual Texture * CreateTexture(const std::string & imagePath) override;
		virtual Shader * CreateShader(const std::string & vPath, const std::string & fPath) override;
		virtual RenderBuffer * CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices, std::uint32_t indices_size, std::uint32_t elementCnt) override;

		virtual void Draw(DrawMode mode, std::uint32_t counts) override;

	private:
		// The actuall draw call
		virtual void Invalid() override;

		GLFWwindow * mWindow;

		std::vector<OpenGLShader*> mShaders;
		std::vector<OpenGLTexture*> mTextures;
		std::vector<OpenGLRenderBuffer*> mBuffers;
	};

}

