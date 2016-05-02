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
	class RenderBuffer;
	class D3DTexture;
	class D3DShader;
	class D3DRenderBuffer;
	class D3DRenderDevice final : public Library::RenderDevice
	{
	public:
		D3DRenderDevice(HWND window, UINT screenWidth, UINT screenHeight);
		virtual ~D3DRenderDevice();


		virtual Library::Viewport * CreateViewport() override;
		virtual Library::Texture * CreateTexture(const std::string & imagePath) override;
		virtual Library::Shader * CreateShader(const std::string & vPath, const std::string & fPath) override;
		virtual Library::RenderBuffer * CreateBuffer(float * data, std::uint32_t size, std::uint32_t stride,
			std::uint32_t * indices = nullptr, std::uint32_t indices_size = 0, std::uint32_t elementCnt = 0) override;

		virtual void Draw(DrawMode mode, std::uint32_t counts) override;

		virtual void ClearScreen();

	private:
		D3D_FEATURE_LEVEL mFeatureLevel;
		ID3D11Device1* mDirect3DDevice;
		ID3D11DeviceContext1* mDirect3DDeviceContext;
		IDXGISwapChain1* mSwapChain;
		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11RenderTargetView* mRenderTargetView;
		ID3D11DepthStencilView* mDepthStencilView;
		void InitializeDirectX(HWND window, UINT mScreenWidth, UINT mScreenHeight);
		XMVECTORF32 BackgroundColor = { 0.392f, 0.584f, 0.929f, 1.0f };

		OldPolygonRenderer* poly;

		std::vector<D3DShader*> mShaders;
		std::vector<D3DTexture*> mTextures;
		std::vector<D3DRenderBuffer*> mBuffers;
	};
}

