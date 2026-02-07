#include "pch.h"
#include "d3dx12.h"
#include <iostream>

#include "RenderDevice.h"
#include "Geometry.h"
#include "Factory.h"
#include "CommandContext.h"
#include "MeshBuffer.h"
#include "Mesh.h"
#include "Window.h"
#include "SwapChain.h"


RenderDevice::RenderDevice()
{

}

bool RenderDevice::Init(Window* _mainWindow)
{
	m_window = _mainWindow;
	//First Stage of initialization : Factory -> Adapter -> Device 
	// DebugLayer doesnt work
	if (InitPhaseState())
	{
		std::cout << "Phase 1 Failed" << std::endl;
		return 1;
	}

	if(InitPhaseExecution())
	{
		std::cout << "Phase 2 Failed" << std::endl;
		return 1;
	}

	if(InitPhaseRender())
	{
		std::cout << "Phase 3 Failed" << std::endl;
		return 1;
	}
	if(InitPhaseClose())
	{
		std::cout << "Phase 4 Failed" << std::endl;
		return 1;
	}
	//init MeshBuffer
	m_MeshBuffer = new MeshBuffer(m_device, m_command->m_list);

	return 0;
}

bool RenderDevice::InitPhaseState()
{
	//build the device if the default adapter isnt compatible then i call my factory to find a adapteur for the device because device is VERY usefull XD
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	m_factory = new Factory();
	m_factory->InitFactory();
	if (FAILED(hr))
	{
		if (m_factory->InitAdapter())
		{
			if (hr = D3D12CreateDevice(m_factory->GAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));//pray this works ig
			{
				std::cout << "Device creation failed" << std::endl; //no consol yet but hopefully I make one
				return 1;
			}
		}

	}
	return 0;
}

bool RenderDevice::InitPhaseExecution()
{
	m_command = new CommandContext;
	if (m_device == nullptr)
	{
		std::cout << "Device Required" << std::endl;
		return 1;
	}
	//init CommandQueue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;


	HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
	{
		std::cout << "Command Queue creation failed" << std::endl;
		return 1;
	}
	//Command allocator creation
	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command->m_allocators[0]));
	if (FAILED(hr))
	{
		std::cout << "First Command allocator creation failed" << std::endl;
		return 1;
	}

	hr = NULL; //reset hr for next call

	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command->m_allocators[1]));
	if (FAILED(hr))
	{
		std::cout << "Second Command allocator creation failed" << std::endl;
		return 1;
	}

	hr = NULL; //reset hr for next call

	hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_command->m_allocators[0], nullptr, IID_PPV_ARGS(&m_command->m_list));
	if (FAILED(hr))
	{
		std::cout << "Command list creation failed" << std::endl;
		return 1;
	}

	m_command->m_list->Close(); //close the command list because i must apparantly
}

bool RenderDevice::InitPhaseRender()
{

	//swapchain
	m_swapChain = new SwapChain();
	IDXGISwapChain1* tempSwapChain = nullptr;

	DXGI_SWAP_CHAIN_DESC1 swapCDesc = m_swapChain->BuildSCDesc(m_window->GCwidth(), m_window->GCheight());

	HRESULT hr = m_factory->GFactory()->CreateSwapChainForHwnd(m_queue, m_window->GHWND(), &swapCDesc,
		nullptr, nullptr, &tempSwapChain);
	if (FAILED(hr))
	{
		std::cout << "Failed to Build SwapChain" << std::endl;
		return 1;
	}

	IDXGISwapChain3* swapchainref = nullptr;

	tempSwapChain->QueryInterface(IID_PPV_ARGS(&swapchainref)); // because i need acces to GetCurrentBackBufferIndex() so i cast to newer version

	m_swapChain->SetSwapChain(swapchainref);
	tempSwapChain->Release();

	//HeapDescriptor
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2; // One for each buffer
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));

	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT i = 0; i < 2; i++)
	{
		m_swapChain->GSwapChain()->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));

		m_device->CreateRenderTargetView(m_renderTargets[i], nullptr, rtvHandle);

		rtvHandle.Offset(1, m_rtvDescriptorSize);
	}
}

bool RenderDevice::InitPhaseClose()
{
	m_fenceValue = 1; // Start at 1
	HRESULT hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr))
	{
		return 1;
	}
	// Create an event handle to use for waiting
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	return 0;
}

void RenderDevice::Render()
{
	m_command->m_list->Reset(m_command->m_allocators[0], nullptr);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	m_command->m_list->ResourceBarrier(1, &barrier);

	Draw();

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_command->m_list->ResourceBarrier(1, &barrier);
	m_command->m_list->Close();

	ID3D12CommandList* ppCommandLists[] = { m_command->m_list };
	m_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	m_swapChain->GSwapChain()->Present(1, 0);
}

void RenderDevice::Run()
{
	MSG msg = { 0 };
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			// 1. Record Commands (Clear screen, Draw Object)
			// 2. Execute Command List
			// 3. Present Swap Chain
			// 4. Wait for GPU (Signal Fence)

			Render();
		}
	}
}

void RenderDevice::Draw()
{

}

void RenderDevice::WaitForGpu()
{
	const UINT64 fenceToWait = m_fenceValue;
	m_queue->Signal(m_fence, fenceToWait);
	m_fenceValue++;

	if (m_fence->GetCompletedValue() < fenceToWait)
	{
		m_fence->SetEventOnCompletion(fenceToWait, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}



