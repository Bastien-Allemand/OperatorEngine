#pragma once


class CommandContext 
{
public:
    ID3D12GraphicsCommandList* m_list;
    ID3D12CommandAllocator* m_allocators[2];

    void Open(int frameIndex);
    void CloseAndExecute(ID3D12CommandQueue* queue);
};

