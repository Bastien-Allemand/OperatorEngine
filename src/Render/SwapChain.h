#pragma once
class SwapChain
{
public:
	SwapChain();
	DXGI_SWAP_CHAIN_DESC1 BuildSCDesc(int _width, int _height);
	IDXGISwapChain3* GSwapChain() const { return m_swapChain; }
	void SetSwapChain(IDXGISwapChain3* _swapChain) { m_swapChain = _swapChain; }
private:
	IDXGISwapChain3* m_swapChain;
};

