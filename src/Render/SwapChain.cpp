#include "pch.h"
#include "SwapChain.h"

SwapChain::SwapChain()
{
}

bool SwapChain::Init(int _width, int _height, HWND _handle, IDXGIFactory4* _factory, ID3D12CommandQueue* _queue)
{
	IDXGISwapChain1* tempSwapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = 2;
	swapChainDesc.Width = _width;
	swapChainDesc.Height = _height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;

	HRESULT hr = _factory->CreateSwapChainForHwnd(_queue, _handle, &swapChainDesc,nullptr, nullptr, &tempSwapChain);
	if (FAILED(hr))
	{
		std::cout << "Failed to Build SwapChain" << std::endl;
		return 1;
	}

	IDXGISwapChain3* swapchainref = nullptr;
	hr = tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapchainref)); // because i need acces to GetCurrentBackBufferIndex() so i cast to newer version
	if (FAILED(hr))
	{
		std::cout << "Failed to cast swapchain to version 3" << std::endl;
		return 1;
	}

	m_swapChain = swapchainref;
	tempSwapChain->Release();

	return 0;
}