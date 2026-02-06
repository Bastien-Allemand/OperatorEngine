#pragma once
class SwapChain
{
public:
	SwapChain();
	void Init();
private:
	IDXGISwapChain* m_swapChain;
};

