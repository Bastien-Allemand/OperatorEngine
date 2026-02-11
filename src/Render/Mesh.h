#pragma once
class Geometry;
class CommandContext;
class RenderDevice;

class Mesh {
    ID3D12Resource* m_vBuffer;
    ID3D12Resource* m_iBuffer;
	ID3D12Resource* m_vUpload;
	ID3D12Resource* m_iUpload;
    D3D12_VERTEX_BUFFER_VIEW m_vbView;
    D3D12_INDEX_BUFFER_VIEW m_ibView;

    void Upload(ID3D12Device& _rd,Geometry& geo, CommandContext& ctx);
    void Bind(CommandContext& ctx);
	void UploadIndex(ID3D12Device& _rd, Geometry& geo, CommandContext& ctx);
	void UploadVertex(ID3D12Device& _rd, Geometry& geo, CommandContext& ctx);
};