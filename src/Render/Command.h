#pragma once
class Command
{
public:
	Command(ID3D12Device* _device);

	void AddResourceBarrier(UINT _numBarriers,const D3D12_RESOURCE_BARRIER* _barrier) { m_commandList->ResourceBarrier(_numBarriers, _barrier); }


	ID3D12CommandAllocator* GCommandAllocator() const { return m_commandAllocator; }
	ID3D12GraphicsCommandList* GCommandList() const { return m_commandList; }
private:
	ID3D12CommandAllocator* m_commandAllocator;
	ID3D12GraphicsCommandList* m_commandList;
	ID3D12CommandQueue* m_commandQueue;
};

