#include "pch.h"
#include "RenderEngine.h"
#include "d3dx12.h"

#include "Factory.h"
#include "RenderDevice.h"
#include "CommandContext.h"
#include "PipelineStateObject.h"
#include "SwapChain.h"
#include "Fence.h"


RenderEngine::~RenderEngine()
{
	//::TODO: release resources
}

bool RenderEngine::Init(int _width, int _height, HWND _handle)
{
	m_factory = new Factory();
	m_renderDevice = new RenderDevice();
	m_commandContext = new CommandContext();
	m_pipelineStateObject = new PipelineStateObject();
	m_swapChain = new SwapChain();
	m_fence = new Fence();

	//init factory
	if (m_factory->InitFactory())
	{
		std::cout << "Failed to initialize factory" << std::endl;
		return 1;
	}

	//init render device
	if (m_renderDevice->Init(m_factory))
	{
		std::cout << "Failed to initialize render device" << std::endl;
		return 1;
	}

	//init command context
	if (m_commandContext->Init(m_renderDevice))
	{
		std::cout << "Failed to initialize command context" << std::endl;
		return 1;
	}

	//init queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr = m_renderDevice->GDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
	{
		std::cout << "Command Queue creation failed" << std::endl;
		return 1;
	}

	//init swapchain
	if (m_swapChain->Init(_width, _height, _handle,m_factory->GFactory(),m_queue))
	{
		std::cout << "Failed to initialize swapchain" << std::endl;
		return 1;
	}

	//init descriptor heap for render target views
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2; // One for each buffer
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_renderDevice->GDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr))
	{
		std::cout << "Failed to create RTV descriptor heap" << std::endl;
		return 1;
	}

	hr = m_rtvDescriptorSize = m_renderDevice->GDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (FAILED(hr))
	{
		std::cout << "Failed to get RTV descriptor size" << std::endl;
		return 1;
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

	// Create root signature
	
	m_pipelineStateObject->Init(m_renderDevice->GDevice());

	m_fence->Init(m_renderDevice->GDevice());
	// Create a RTV for each frame.
	// todo after RenderTargets are created
	//for (UINT i = 0; i < 2; i++)
	//{
	//	m_swapChain->GSwapChain()->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));

	//	m_renderDevice->GDevice()->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);

	//	rtvHandle.Offset(1, m_rtvDescriptorSize);
	//}

}

void RenderEngine::Update()
{
	m_commandContext->GCommandAllocator()->Reset();
	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), m_pipelineStateObject->GPipelineState());

	//auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	//m_commandContext->GCommandList()->ResourceBarrier(1, &barrier);

	//draw calls would go here

	//barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	//m_commandContext->GCommandList()->ResourceBarrier(1, &barrier);
	m_commandContext->CloseAndExecute(m_queue);

	m_swapChain->GSwapChain()->Present(1, 0);

	m_fence->WaitForCompletion(m_queue);
}
