#pragma once

///foward dec
class DebugLayer;


class RenderDevice
{
public:
	RenderDevice();

	int InitWindow(int _width, int _height, const wchar_t* _title, WNDCLASS _windowsClass, HINSTANCE _hInstance, int _cmdShow);
	bool InitDebugLayer();
	bool InitDXGIFactory();
	void InitAdapter();
	void InitD3D12();
	bool InitDevice();
	void Update();
	void Draw();

	ID3D12Device* GetDevice() const { return m_device; }

private:

	DebugLayer* m_debugLayer;
	IDXGIFactory4* m_factory;
	IDXGIAdapter1* m_adapter;
	ID3D12Device* m_device;

	ID3D12Resource* m_vertexBuffer;

	//IDXGISwapChain* m_swapChain;
	//ID3D12CommandQueue* m_commandQueue;
	//ID3D12CommandAllocator* m_commandAllocator;
	//ID3D12GraphicsCommandList* m_commandList;
};

