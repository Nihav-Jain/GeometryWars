#pragma once
namespace Library {

	class Viewport;
	class Shader;
	class Texture;
	class RenderBuffer;
	/**
	* Render device object, for OpenGL and D3D implementation
	*/
	class RenderDevice
	{
	public:
		RenderDevice();
		virtual ~RenderDevice();

		virtual Viewport * CreateViewport() = 0;

		// TODO: We might need Texture2D class to handle this, this is a quick and messy solution
		// D3D: ID3D11Device::CreateTexture2D
		// OpenGL: SOIL library
		virtual std::uint32_t CreateTexture(const std::string & imagePath) = 0;
		virtual void UseTexture(std::uint32_t) = 0;

		virtual Shader * CreateShader(const std::string & vPath, const std::string & fPath) = 0;

		// TODO: Buffer class
		virtual std::uint32_t CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride) = 0;
		virtual void UseBuffer(std::uint32_t) = 0;

		virtual void Draw() = 0;

		// The actual drawing call
		virtual void Invalid() = 0;
	};

}