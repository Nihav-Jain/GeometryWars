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
		virtual Texture * CreateTexture(const std::string & imagePath) = 0;
		virtual Shader * CreateShader(const std::string & vPath, const std::string & fPath) = 0;
		virtual RenderBuffer * CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride) = 0;

		virtual void Draw() = 0;

		// The actual drawing call
		virtual void Invalid() = 0;
	};

}