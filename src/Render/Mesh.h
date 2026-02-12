#pragma once
#include "Geometry.h"
class CommandContext;
class RenderDevice;
#include <unordered_map>


struct SubmeshGeometry
{
    UINT IndexCount = 0;
    UINT StartIndexLocation = 0;
    INT BaseVertexLocation = 0;

    // Bounding box of the geometry defined by this submesh. 
    // This is used in later chapters of the book.
    DirectX::BoundingBox Bounds;
};

class Mesh {
public:
    Mesh(Geometry _geo);
    size_t GetIndexCount() const;
    bool Upload(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
    void Bind(ID3D12GraphicsCommandList* _list);

    std::unordered_map<std::string, SubmeshGeometry> DrawArgs;

private:
    ID3DBlob* m_vBuffer;
    ID3DBlob* m_iBuffer;
    ID3D12Resource* m_vGPUBuffer;
    ID3D12Resource* m_iGPUBuffer;
	ID3D12Resource* m_vUpload;
	ID3D12Resource* m_iUpload;

    UINT VertexByteStride = 0;
    UINT VertexBufferByteSize = 0;
    DXGI_FORMAT IndexFormat = DXGI_FORMAT_R16_UINT;
    UINT IndexBufferByteSize = 0;

    Geometry m_geo;
	void UploadIndex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	void UploadVertex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	ID3D12Resource* CreateDefaultBuffer(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list, const void* _data, UINT64 _byteSize, ID3D12Resource* _uploadBuffer);
	bool m_isUploaded = false;
};