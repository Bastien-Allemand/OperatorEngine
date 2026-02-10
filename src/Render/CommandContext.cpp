#include "pch.h"
#include "d3dx12.h"
#include "CommandContext.h"
#include "RenderDevice.h"

bool CommandContext::Init(RenderDevice* _renderDevice)
{
	HRESULT hr = _renderDevice->GDevice()->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_allocators));
	if (FAILED(hr))
	{
		std::cout << "First Command allocator creation failed" << std::endl;
		return 1;
	}

	hr = NULL; //reset hr for next call

	hr = _renderDevice->GDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_allocators, nullptr, IID_PPV_ARGS(&m_list));
	if (FAILED(hr))
	{
		std::cout << "Command list creation failed" << std::endl;
		return 1;
	}
	m_list->Close(); //close the command list because i must apparantly
	return 0;
}

void CommandContext::CloseAndExecute(ID3D12CommandQueue* _queue)
{
	m_list->Close();
	ID3D12CommandList* ppCommandLists[] = { m_list };
	_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}
