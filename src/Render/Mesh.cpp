#include "pch.h"
#include "Mesh.h"
#include "Geometry.h"
#include "CommandContext.h"

void Mesh::Upload(ID3D12Device& _rd,Geometry& geo, CommandContext& ctx)
{
	UploadVertex(_rd, geo, ctx);
	UploadIndex(_rd, geo, ctx);
}

void Mesh::Bind(CommandContext& ctx)
{
}

void Mesh::UploadIndex(ID3D12Device& _rd, Geometry& geo, CommandContext& ctx)
{
	const uint32 iBufferSize = static_cast<uint32>(geo.indices.size() * sizeof(unsigned int));

	auto defaultProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto iDesc = CD3DX12_RESOURCE_DESC::Buffer(iBufferSize);
	_rd.CreateCommittedResource(&defaultProp, D3D12_HEAP_FLAG_NONE, &iDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_iBuffer));

	auto uploadProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	_rd.CreateCommittedResource(&uploadProp, D3D12_HEAP_FLAG_NONE, &iDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_iUpload));

	void* pData;
	m_iUpload->Map(0, nullptr, &pData);
	memcpy(pData, geo.indices.data(), iBufferSize);
	m_iUpload->Unmap(0, nullptr);

	ctx.GCommandList()->CopyBufferRegion(m_iBuffer, 0, m_iUpload, 0, iBufferSize);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_iBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
	ctx.GCommandList()->ResourceBarrier(1, &barrier);

	m_ibView.BufferLocation = m_iBuffer->GetGPUVirtualAddress();
	m_ibView.SizeInBytes = iBufferSize;
	m_ibView.Format = DXGI_FORMAT_R32_UINT; // Essential for DrawIndexed

}

void Mesh::UploadVertex(ID3D12Device& _rd, Geometry& geo, CommandContext& ctx)
{
	const uint32 vBufferSize = static_cast<uint32>(geo.vertices.size() * sizeof(geo.vertices[0]));


	auto defaultProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto vDesc = CD3DX12_RESOURCE_DESC::Buffer(vBufferSize);
	_rd.CreateCommittedResource(&defaultProp, D3D12_HEAP_FLAG_NONE, &vDesc,
		D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_vBuffer));

	auto uploadProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	_rd.CreateCommittedResource(&uploadProp, D3D12_HEAP_FLAG_NONE, &vDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_vUpload));

	void* pData;
	m_vUpload->Map(0, nullptr, &pData);
	memcpy(pData, geo.vertices.data(), vBufferSize);
	m_vUpload->Unmap(0, nullptr);

	ctx.GCommandList()->CopyBufferRegion(m_vBuffer, 0, m_vUpload, 0, vBufferSize);

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_vBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
	ctx.GCommandList()->ResourceBarrier(1, &barrier);

	m_vbView.BufferLocation = m_vBuffer->GetGPUVirtualAddress();
	m_vbView.StrideInBytes = sizeof(vertex);
	m_vbView.SizeInBytes = vBufferSize;
}
