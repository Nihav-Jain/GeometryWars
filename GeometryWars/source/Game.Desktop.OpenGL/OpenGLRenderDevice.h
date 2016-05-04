#pragma once
#include "RenderDevice.h"
#include "OpenGLShader.h"
#include "OpenGLTexture.h"
#include "OpenGLBuffer.h"
#include "OpenGLFrameBuffer.h"
#include <vector>

class GLFWwindow;

namespace OpenGLImplmentation {

	class OpenGLRenderDevice final :
		public Library::RenderDevice
	{
	public:
		OpenGLRenderDevice();
		virtual ~OpenGLRenderDevice();

		OpenGLRenderDevice(const OpenGLRenderDevice &) = delete;
		OpenGLRenderDevice & operator=(const OpenGLRenderDevice &) = delete;

		void InitOpenGl(const std::string & title, std::int32_t width, std::int32_t height);

		virtual std::int32_t GetViewportWidth() override;
		virtual std::int32_t GetViewportHeight() override;
		virtual Library::Texture * CreateTexture(const std::string & imagePath) override;
		virtual Library::Shader * CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath) override;
		virtual Library::Buffer * CreateBuffer(bool createIndicesBuffer) override;
		virtual Library::FrameBuffer * CreateFrameBuffer(std::uint32_t textureCnt) override;
		virtual Library::FrameBuffer * GetDefaultFrameBuffer() override;

		virtual void Draw(DrawMode mode, std::uint32_t counts, bool useIndices) override;

		virtual bool IsValid();

	private:
		// The actuall draw call
		virtual void ClearScreen() override;

		GLFWwindow * mWindow;

		std::vector<OpenGLShader*> mShaders;
		std::vector<OpenGLTexture*> mTextures;
		std::vector<OpenGLRenderBuffer*> mBuffers;

		std::int32_t mWidth;
		std::int32_t mHeight;
	};

}

