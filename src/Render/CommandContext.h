#pragma once


class CommandContext 
{
public:
    ID3D12GraphicsCommandList* m_list;
    ID3D12CommandAllocator* m_allocators;

    void Open(int _frameIndex);
    void CloseAndExecute(ID3D12CommandQueue* _queue);
};

