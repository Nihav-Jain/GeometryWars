#pragma once
#include <RenderDevice.h>

struct ID3D11Device1;
struct ID3D11DeviceContext;
struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11InputLayout;
struct ID3D11Buffer;
class OldPolygonRenderer;
namespace Library
{
	class Viewport;
	class Texture;
	class Shader;
	class Buffer;
	class D3DTexture;
	class D3DShader;
	class D3DRenderBuffer;
	class D3DRenderDevice final : public RenderDevice
	{
	public:
		D3DRenderDevice();
		virtual ~D3DRenderDevice();

		void InitializeDirectX(HWND window, UINT mScreenWidth, UINT mScreenHeight);

		virtual Library::Texture * CreateTexture(const std::string & imagePath) override;
		virtual Library::Shader * CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath) override;
		virtual Library::Buffer * CreateBuffer(bool createIndicesBuffer) override;
		virtual FrameBuffer * CreateFrameBuffer(std::uint32_t textureCnt) override { UNREFERENCED_PARAMETER(textureCnt); return nullptr; };
		virtual FrameBuffer * GetDefaultFrameBuffer() override { return nullptr; };

		virtual std::int32_t GetViewportWidth() override;
		virtual std::int32_t GetViewportHeight() override;

		virtual void Draw(DrawMode mode, std::uint32_t counts, bool useIndices) override;

		virtual void ClearScreen();

	private:
		D3D_FEATURE_LEVEL mFeatureLevel;
		ID3D11Device1* mDirect3DDevice;
		ID3D11DeviceContext1* mDirect3DDeviceContext;
		IDXGISwapChain1* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;

		//OldPolygonRenderer* poly;

		std::vector<D3DShader*> mShaders;
		std::vector<D3DTexture*> mTextures;
		std::vector<D3DRenderBuffer*> mBuffers;
		std::int32_t mWidth;
		std::int32_t mHeight;
	};
}

