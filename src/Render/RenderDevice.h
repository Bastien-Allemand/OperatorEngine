#pragma once

///foward dec
class Factory;
class CommandContext;
class MeshBuffer;
class Window;
class SwapChain;
class PipelineStateObject;

class RenderDevice
{
public:
	RenderDevice();

	bool Init(Window* _mainWindow);

	void Run();

	void Draw();

private:
	CommandContext* m_command;

	MeshBuffer* m_MeshBuffer;

	Factory* m_factory;

	Window* m_window;

	PipelineStateObject* m_pso;
	//

	ID3D12Device* m_device;
	ID3D12CommandQueue* m_queue;

	SwapChain* m_swapChain;

	ID3D12DescriptorHeap* m_rtvHeap;
	UINT m_rtvDescriptorSize;

	ID3D12Resource* m_renderTargets[2];
	UINT m_frameIndex;

	ID3D12RootSignature* m_rootSig;

	ID3D12Fence* m_fence;
	UINT64 m_fenceValue;
	HANDLE m_fenceEvent;

	void Render();
	bool InitPhaseState();
	bool InitPhaseExecution();
	bool InitPhaseRender();
	bool InitPhaseClose();
	bool InitPhasePso();
	void WaitForGpu();
};

