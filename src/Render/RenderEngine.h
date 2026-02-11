#pragma once

class Mesh;
class Fence;
class Factory;
class SwapChain;
class Descriptors;
class RenderDevice;
class RenderTarget;
class CommandContext;
class PipelineStateObject;

class RenderEngine
{
public:
	RenderEngine() = default;
	~RenderEngine();
	bool Init(int _width, int _height, HWND _handle);
	void Update();
	bool Resize(int _width, int _height);
	void AddMeshToDraw(Mesh* _mesh) { m_meshes.push_back(_mesh); }
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

	Vector<Mesh*> m_meshes;

	Mesh* m_quadMesh = nullptr;

	bool FlushCommandQueue();
};

