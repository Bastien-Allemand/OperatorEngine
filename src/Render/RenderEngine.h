#pragma once

class CommandContext;
class PipelineStateObject;
class SwapChain;
class RenderDevice;
class Factory;
class Fence;
class Descriptors;
class RenderTarget;

class RenderEngine
{
public:
	RenderEngine() = default;
	~RenderEngine();
	bool Init(int _width, int _height, HWND _handle);
	void Update();
	bool Resize(int _width, int _height);

	bool m4xMsaaState = 0;
	uint32 m4xMsaaQuality = 0;

private:
	Factory* m_factory = nullptr;

	RenderDevice* m_renderDevice = nullptr;

	CommandContext* m_commandContext = nullptr;

	PipelineStateObject* m_pipelineStateObject = nullptr;

	SwapChain* m_swapChain = nullptr;

	ID3D12CommandQueue* m_queue = nullptr;

	Descriptors* m_desc;

	RenderTarget* m_renderTarget;

	Fence* m_fence = nullptr;


	bool FlushCommandQueue();
};

