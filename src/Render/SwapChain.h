#pragma once
class SwapChain
{
public:
	SwapChain();
	bool Init(int _width, int _height, HWND _handle, IDXGIFactory4* _factory, ID3D12CommandQueue* _queue);

	IDXGISwapChain3* GSwapChain() const { return m_swapChain; }

private:
	IDXGISwapChain3* m_swapChain = nullptr;
};

