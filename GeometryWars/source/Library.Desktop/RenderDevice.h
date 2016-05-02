#pragma once
namespace Library {

	class Viewport;
	class Shader;
	class Texture;
	class Buffer;
	class FrameBuffer;
	/**
	* Render device object, for OpenGL and D3D implementation
	*/
	class RenderDevice
	{
	public:

		enum class DrawMode {
			TRIANGLES,
			LINES,
			POINTS
		};

		RenderDevice() = default;
		virtual ~RenderDevice() = default;

		virtual std::int32_t GetViewportWidth() = 0 ;
		virtual std::int32_t GetViewportHeight() = 0;
		virtual Texture * CreateTexture(const std::string & imagePath) = 0;
		virtual Shader * CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath) = 0;
		virtual Buffer * CreateBuffer(bool createIndicesBuffer) = 0;
		virtual Library::FrameBuffer * CreateFrameBuffer() = 0;

		virtual void Draw(DrawMode mode = DrawMode::TRIANGLES, std::uint32_t counts = 6) = 0;

		// The actual drawing call
		virtual void Invalid() = 0;
	};

}