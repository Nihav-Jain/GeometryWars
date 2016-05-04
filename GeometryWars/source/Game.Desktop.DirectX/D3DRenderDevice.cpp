#include "pch.h"
#include "D3DRenderDevice.h"
#include "D3DTexture.h"
#include "D3DShader.h"
#include "D3DRenderBuffer.h"
#include "OldPolygonRenderer.h"

//OldPolygonRenderer* opr;
namespace Library
{
	D3DRenderDevice::D3DRenderDevice() : mDirect3DDevice(nullptr), mDirect3DDeviceContext(nullptr), mSwapChain(nullptr),
		mDepthStencilBuffer(nullptr), mRenderTargetView(nullptr), mDepthStencilView(nullptr),// poly(nullptr),
		mWidth(800), mHeight(600)
	{
		//opr = new OldPolygonRenderer(*mDirect3DDevice, *mDirect3DDeviceContext);
	}


	D3DRenderDevice::~D3DRenderDevice()
	{
		for (auto ptr : mShaders) {
			delete ptr;
		}

		for (auto ptr : mBuffers) {
			delete ptr;
		}

		for (auto ptr : mTextures) {
			delete ptr;
		}
		ReleaseObject(mDirect3DDevice);
		ReleaseObject(mDirect3DDeviceContext);
		ReleaseObject(mSwapChain);
		ReleaseObject(mDepthStencilBuffer);
		ReleaseObject(mRenderTargetView);
		ReleaseObject(mDepthStencilView);
	}

	std::int32_t D3DRenderDevice::GetViewportWidth()
	{
		return mWidth;
	}

	std::int32_t D3DRenderDevice::GetViewportHeight()
	{
		return mHeight;
	}

	Texture * D3DRenderDevice::CreateTexture(const std::string & imagePath)
	{
		D3DTexture * texture = new D3DTexture(*mDirect3DDevice, *mDirect3DDeviceContext);
		texture->Init(imagePath);
		mTextures.push_back(texture);
		return texture;
	}

	Shader * D3DRenderDevice::CreateShader(const std::string & vPath, const std::string & fPath, const std::string & gPath)
	{
		D3DShader * shader = new D3DShader(*mDirect3DDevice, *mDirect3DDeviceContext);
		shader->Init(vPath, fPath, gPath);
		mShaders.push_back(shader);
		return shader;
	}

	Buffer * D3DRenderDevice::CreateBuffer(bool createIndicesBuffer)
	{
		D3DRenderBuffer * buffer = new D3DRenderBuffer(*mDirect3DDevice, *mDirect3DDeviceContext);
		buffer->Init(createIndicesBuffer);
		mBuffers.push_back(buffer);
		return buffer;
	}

	void D3DRenderDevice::Draw(DrawMode mode, std::uint32_t counts, bool useIndicesBuffer)
	{

		switch (mode)
		{
		case Library::RenderDevice::DrawMode::TRIANGLES:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			break;
		case Library::RenderDevice::DrawMode::LINES:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			break;
		case Library::RenderDevice::DrawMode::POINTS:
			mDirect3DDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			break;
		default:
			break;
		}
		if(useIndicesBuffer)
			mDirect3DDeviceContext->DrawIndexed(counts, 0, 0);
		else
			mDirect3DDeviceContext->Draw(counts, 0);
		//opr->Draw();
	}

	void D3DRenderDevice::ClearScreen()
	{
		//static float hue = 0;
		static XMVECTORF32 BackgroundColor = { 0, 0, 0, 1.0f };
		//hue += 0.001f;
		//BackgroundColor = { std::sin(hue), std::sin(hue + 6.28f/3), std::sin(hue + 2 * 6.28f / 3), 1.0f };
		ThrowIfFailed(mSwapChain->Present(0, 0), "IDXGISwapChain::Present() failed.");
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView, reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}


	void D3DRenderDevice::InitializeDirectX(HWND window, UINT mScreenWidth, UINT mScreenHeight)
	{
		mWidth = mScreenWidth;
		mHeight = mScreenHeight;
		UINT createDeviceFlags = 0;
	
	#if defined(DEBUG) || defined(_DEBUG)  
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif
	
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0
		};
	
		ID3D11Device* direct3DDevice = nullptr;
		ID3D11DeviceContext* direct3DDeviceContext = nullptr;
		ThrowIfFailed(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, createDeviceFlags, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &direct3DDevice, &mFeatureLevel, &direct3DDeviceContext), "D3D11CreateDevice() failed");
	
		ThrowIfFailed(direct3DDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&mDirect3DDevice)), "ID3D11Device::QueryInterface() failed");
		ThrowIfFailed(direct3DDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&mDirect3DDeviceContext)), "ID3D11Device::QueryInterface() failed");
	
		ReleaseObject(direct3DDevice);
		ReleaseObject(direct3DDeviceContext);
	
		UINT multiSamplingCount = 4;
		UINT multiSamplingQualityLevels;
		mDirect3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, multiSamplingCount, &multiSamplingQualityLevels);
		if (multiSamplingQualityLevels == 0)
		{
			throw GameException("Unsupported multi-sampling quality");
		}
	
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
		ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
		swapChainDesc.Width = mScreenWidth;
		swapChainDesc.Height = mScreenHeight;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = multiSamplingCount;
		swapChainDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 1;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	
		IDXGIDevice* dxgiDevice = nullptr;
		ThrowIfFailed(mDirect3DDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&dxgiDevice)), "ID3D11Device::QueryInterface() failed");
	
		IDXGIAdapter *dxgiAdapter = nullptr;
		HRESULT hr;
		if (FAILED(hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&dxgiAdapter))))
		{
			ReleaseObject(dxgiDevice);
			throw GameException("IDXGIDevice::GetParent() failed retrieving adapter.", hr);
		}
	
		IDXGIFactory2* dxgiFactory = nullptr;
		if (FAILED(hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory))))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			throw GameException("IDXGIAdapter::GetParent() failed retrieving factory.", hr);
		}
	
		DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullScreenDesc;
		ZeroMemory(&fullScreenDesc, sizeof(fullScreenDesc));
		fullScreenDesc.RefreshRate.Numerator = 60;
		fullScreenDesc.RefreshRate.Denominator = 1;
		fullScreenDesc.Windowed = true;
	
		if (FAILED(hr = dxgiFactory->CreateSwapChainForHwnd(dxgiDevice, window, &swapChainDesc, &fullScreenDesc, nullptr, &mSwapChain)))
		{
			ReleaseObject(dxgiDevice);
			ReleaseObject(dxgiAdapter);
			ReleaseObject(dxgiFactory);
			throw GameException("IDXGIDevice::CreateSwapChainForHwnd() failed.", hr);
		}
	
		ReleaseObject(dxgiDevice);
		ReleaseObject(dxgiAdapter);
		ReleaseObject(dxgiFactory);
	
		ID3D11Texture2D* backBuffer;
		ThrowIfFailed(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)), "IDXGISwapChain::GetBuffer() failed.");
		if (FAILED(hr = mDirect3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView)))
		{
			ReleaseObject(backBuffer);
			throw GameException("IDXGIDevice::CreateRenderTargetView() failed.", hr);
		}
	
		ReleaseObject(backBuffer);
	
		D3D11_TEXTURE2D_DESC depthStencilDesc;
		ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
		depthStencilDesc.Width = mScreenWidth;
		depthStencilDesc.Height = mScreenHeight;
		depthStencilDesc.MipLevels = 1;
		depthStencilDesc.ArraySize = 1;
		depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		depthStencilDesc.SampleDesc.Count = multiSamplingCount;
		depthStencilDesc.SampleDesc.Quality = multiSamplingQualityLevels - 1;
	
		ThrowIfFailed(mDirect3DDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer), "IDXGIDevice::CreateTexture2D() failed.");
		ThrowIfFailed(mDirect3DDevice->CreateDepthStencilView(mDepthStencilBuffer, nullptr, &mDepthStencilView), "IDXGIDevice::CreateDepthStencilView() failed.");
	
		D3D11_VIEWPORT viewport;
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;
		viewport.Width = static_cast<float>(mScreenWidth);
		viewport.Height = static_cast<float>(mScreenHeight);
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
	
		mDirect3DDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
		mDirect3DDeviceContext->RSSetViewports(1, &viewport);
	}
}