#include "pch.h"
#include "RenderTarget.h"

RenderTarget::~RenderTarget()
{
	for (int i = 0; i < BufferCount; i++)
	{
		m_backbuffer[i] = nullptr;
	}
	m_DepthStencilBuffer = nullptr;
}

D3D12_CPU_DESCRIPTOR_HANDLE RenderTarget::GCurrentBackBufferView(ID3D12DescriptorHeap* _rtvheap, UINT _rtvsize) const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_rtvheap->GetCPUDescriptorHandleForHeapStart(), m_CurrentBuffer, _rtvsize);
}
