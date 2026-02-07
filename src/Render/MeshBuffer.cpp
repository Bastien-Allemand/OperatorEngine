#include "pch.h"
#include "MeshBuffer.h"
#include "d3dx12.h"
#include <iostream>

MeshBuffer::MeshBuffer(ID3D12Device* _device, ID3D12GraphicsCommandList* _commandList)
{
    const UINT64 vertexBufferSize = 1024;

    // 2. Créer la description de ressource
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    // 3. Propriétés du Heap (Où la ressource habite)
    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    auto heapPropsUpload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

    auto resourceDescUpload = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);
    // 4. Création de la ressource
    HRESULT hr = _device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // On commence en état "Destination de copie"
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)
    );
    if (!hr)
    {
		std::cout << " creation failed" << std::endl;
    }
    hr = _device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // On commence en état "Destination de copie"
        nullptr,
        IID_PPV_ARGS(&m_indexBuffer)
    );
    if (!hr)
    {
        std::cout << " creation failed2" << std::endl;
    }
    hr = _device->CreateCommittedResource(
        &heapPropsUpload,
        D3D12_HEAP_FLAG_NONE,
        &resourceDescUpload,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexUpload)
    );
    if (!hr)
    {
        std::cout << " creation failed3" << std::endl;
    }
}

void MeshBuffer::AddData(void* _data, size_t _size)
{
    m_vertexUpload->Map(0, nullptr, &m_data);
    memcpy(m_data, _data, sizeof(_data));
}
