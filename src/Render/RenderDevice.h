#pragma once

///foward dec
class DebugLayer;
class Factory;
class Command;
class ConstantBuffer;

class RenderDevice
{
public:
	RenderDevice();
	void Init();
	int InitWindow(int _width, int _height, const wchar_t* _title);
	bool InitDevice();
	void InitVertexViewBuffer();
	void Render();
	void Update();
	void Draw();
private:


	DebugLayer* m_debugLayer;
	Factory* m_factory;
	ID3D12Device* m_device;
	Command* m_command;
	ConstantBuffer* m_constantBuffer;

	D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_indexBufferView;

	//IDXGISwapChain* m_swapChain;
};

