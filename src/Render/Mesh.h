#pragma once
class Geometry;
class CommandContext;

class Mesh {
    ID3D12Resource* m_vBuffer;
    ID3D12Resource* m_iBuffer;
    D3D12_VERTEX_BUFFER_VIEW m_vbView;
    D3D12_INDEX_BUFFER_VIEW m_ibView;

    void Upload(Geometry& geo, CommandContext& ctx);
    void Bind(CommandContext& ctx);
};