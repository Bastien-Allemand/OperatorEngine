#pragma once
///foward dec
class DebugLayer;


class RenderDevice
{
public:
	RenderDevice();

	int InitWindow(int _width, int _height, const wchar_t* _title, WNDCLASS _windowsClass, HINSTANCE _hInstance, int _cmdShow);
	void InitDebugLayer();
	void InitDXGIFactory();
	void InitD3D12();

	void Update();
	void Draw();
private:

	DebugLayer* m_debugLayer;


	ID3D12Device* m_device;


	//IDXGISwapChain* m_swapChain;
	//ID3D12CommandQueue* m_commandQueue;
	//ID3D12CommandAllocator* m_commandAllocator;
	//ID3D12GraphicsCommandList* m_commandList;
};

