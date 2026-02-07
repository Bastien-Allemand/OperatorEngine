#include "pch.h"
#include "RenderDevice.h"
#include "d3dx12.h"
#include "Geometry.h"
#include "Factory.h"
#include "CommandContext.h"
#include "MeshBuffer.h"
#include "Mesh.h"
#include <iostream>

RenderDevice::RenderDevice()
{
}

bool RenderDevice::Init()
{
	//First Stage of initialization : Factory -> Adapter -> Device 
	// DebugLayer doesnt work
	InitPhaseState();

	InitPhaseExecution();

	//init CommandContext


	hr = NULL; //reset hr for next call

    hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_command->m_allocators[0], nullptr, IID_PPV_ARGS(&m_command->m_list));
    if (FAILED(hr))
	{
		std::cout << "Command list creation failed" << std::endl;
		return;
	}

	hr = NULL; //reset hr for next call



	//init MeshBuffer
	m_MeshBuffer = new MeshBuffer(m_device, m_command->m_list);


}

bool RenderDevice::InitPhaseState()
{
	return InitDevice();
}

bool RenderDevice::InitPhaseExecution()
{
	//init CommandQueue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HRESULT hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
	{
		std::cout << "Command Queue creation failed" << std::endl;
		return;
	}
	//Command allocator creation
	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command->m_allocators[0]));
	if (FAILED(hr))
	{
		std::cout << "First Command allocator creation failed" << std::endl;
		return;
	}

	hr = NULL; //reset hr for next call

	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_command->m_allocators[1]));
	if (FAILED(hr))
	{
		std::cout << "Second Command allocator creation failed" << std::endl;
		return;
	}
}

bool RenderDevice::InitDevice()
{
   //build the device if the default adapter isnt compatible then i call my factory to find a adapteur for the device because device is VERY usefull XD
	HRESULT hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
	if (FAILED(hr))
	{
		m_factory = new Factory();
		m_factory->Init();
		if(hr = D3D12CreateDevice(m_factory->GAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device)));//pray this works ig
		{
			std::cout << "Device creation failed" << std::endl; //no consol yet but hopefully I make one
			return false;
		}
	}
	return true;
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

	m_swapChain->Present(1, 0);

}

void RenderDevice::Draw()
{

}

void RenderDevice::WaitForGpu()
{

}



