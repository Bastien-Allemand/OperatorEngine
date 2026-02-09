#include "pch.h"
#include "d3dx12.h"
#include "CommandContext.h"

void CommandContext::Open(int _frameIndex)
{
}

void CommandContext::CloseAndExecute(ID3D12CommandQueue* _queue)
{
	m_list->Close();
	ID3D12CommandList* ppCommandLists[] = { m_list };
	_queue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
}
