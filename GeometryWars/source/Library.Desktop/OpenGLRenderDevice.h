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

		virtual std::uint32_t LoadTexture(const std::string & imagePath) override;
		virtual void UseTexture(std::uint32_t) override;

		virtual std::uint32_t LoadShader(const std::string & vPath, const std::string & fPath) override;
		virtual void UseShader(std::uint32_t) override;
		virtual void SetShaderMatrix4(std::uint32_t id, const std::string & name, const glm::mat4 & value) override;
		virtual void SetShaderVector4(std::uint32_t id, const std::string & name, const glm::vec4 & value) override;

		virtual std::uint32_t CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride) override;
		virtual void UseBuffer(std::uint32_t) override;

		virtual void Draw() override;

	private:
		// The actuall draw call
		virtual void Invalid() override;

		GLFWwindow * mWindow;
	};

}

