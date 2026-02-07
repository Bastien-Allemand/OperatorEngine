#pragma once

///foward dec
class Factory;
class CommandContext;
class MeshBuffer;

class RenderDevice
{
public:
	RenderDevice();



	bool Init();
	void Render();

	void Draw();
private:
	CommandContext* m_command;

	MeshBuffer* m_MeshBuffer;

	Factory* m_factory;

	//

	ID3D12Device* m_device;

	ID3D12CommandQueue* m_queue;

	IDXGISwapChain* m_swapChain;

	ID3D12DescriptorHeap* m_rtvHeap;
	UINT m_rtvDescriptorSize;

	ID3D12Resource* m_renderTargets[2];
	UINT m_frameIndex;

	bool InitPhaseState();
	bool InitPhaseExecution();

	bool InitDevice();
	void WaitForGpu();
};

