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

		// TODO: We might need Texture2D class to handle this, this is a quick and messy solution
		// D3D: ID3D11Device::CreateTexture2D
		// OpenGL: SOIL library
		virtual std::uint32_t LoadTexture(const std::string & imagePath) = 0;
		virtual void UseTexture(std::uint32_t) = 0;

		// TODO: Shader class
		virtual std::uint32_t LoadShader(const std::string & vPath, const std::string & fPath) = 0;
		virtual void UseShader(std::uint32_t) = 0;
		virtual void SetShaderMatrix4(std::uint32_t id, const std::string & name, const glm::mat4 & value) = 0;
		virtual void SetShaderVector4(std::uint32_t id, const std::string & name, const glm::vec4 & value) = 0;

		// TODO: Buffer class
		virtual std::uint32_t CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride) = 0;
		virtual void UseBuffer(std::uint32_t) = 0;

		virtual void Draw() = 0;

	private:
		// The actual drawing call
		virtual void Invalid() = 0;
		// TODO: more interface
	};

}