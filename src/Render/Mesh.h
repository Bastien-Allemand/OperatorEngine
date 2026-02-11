#pragma once
class Geometry;
class CommandContext;
class RenderDevice;

class Mesh {
public:
    Mesh(Geometry& _geo);
    size_t GetIndexCount() const;
    void Upload(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
    void Bind(ID3D12GraphicsCommandList* _list);
private:
    ID3D12Resource* m_vBuffer;
    ID3D12Resource* m_iBuffer;
	ID3D12Resource* m_vUpload;
	ID3D12Resource* m_iUpload;
    D3D12_VERTEX_BUFFER_VIEW m_vbView;
    D3D12_INDEX_BUFFER_VIEW m_ibView;
    Geometry& m_geo;
	void UploadIndex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	void UploadVertex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);

	bool m_isUploaded = false;
};