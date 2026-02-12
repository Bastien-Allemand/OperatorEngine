#include "pch.h"
#include "Mesh.h"
#include "Geometry.h"
#include "CommandContext.h"

Mesh::Mesh(Geometry _geo) : m_geo(_geo){}

size_t Mesh::GetIndexCount() const
{
	return m_geo.indices.size();
}

bool Mesh::Upload(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	if (m_isUploaded) return 1;

	UploadVertex(_rd, _list);
	UploadIndex(_rd, _list);
	m_isUploaded = true;

	return 0;
}

void Mesh::Bind(ID3D12GraphicsCommandList* _list)
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = m_vGPUBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = VertexByteStride;
	vbv.SizeInBytes = VertexBufferByteSize;

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = m_iGPUBuffer->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	// On lie les sommets
	_list->IASetVertexBuffers(0, 1, &vbv);
	// On lie les indices
	_list->IASetIndexBuffer(&ibv);
}

void Mesh::UploadIndex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	const UINT ibByteSize = (UINT)m_geo.indices.size() * sizeof(uint32);
	D3DCreateBlob(ibByteSize, &m_iBuffer);

	memcpy(m_iBuffer->GetBufferPointer(), m_geo.indices.data(), ibByteSize);

	m_iGPUBuffer = CreateDefaultBuffer(_rd, _list, m_geo.indices.data(), ibByteSize, m_iUpload);

	IndexFormat = DXGI_FORMAT_R32_UINT;
	IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)m_geo.indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	DrawArgs["box"] = submesh;
}

void Mesh::UploadVertex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	const UINT vbByteSize = (UINT)m_geo.vertices.size() * sizeof(vertex);
	D3DCreateBlob(vbByteSize, &m_vBuffer);

	memcpy(m_vBuffer->GetBufferPointer(), m_geo.vertices.data(), vbByteSize);

	m_vGPUBuffer = CreateDefaultBuffer(_rd, _list, m_geo.vertices.data(), vbByteSize, m_vUpload);

	VertexByteStride = sizeof(vertex);
	VertexBufferByteSize = vbByteSize;
}

ID3D12Resource* Mesh::CreateDefaultBuffer(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list, const void* _data, UINT64 _byteSize, ID3D12Resource* _uploadBuffer)
{
	ID3D12Resource* defaultBuffer = nullptr;
	auto defaultProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	auto bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(_byteSize);

	HRESULT hr = _rd->CreateCommittedResource(&defaultProp, D3D12_HEAP_FLAG_NONE, &bufferDesc,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&defaultBuffer));
	if (FAILED(hr))
	{
		DebugMsg("failed to build default buffer", DebugFlag::WARNING);
	}
	ID3D12Resource* uploadBuffer = nullptr;

	auto uploadProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_byteSize);

	hr = _rd->CreateCommittedResource(&uploadProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&uploadBuffer));
	if (FAILED(hr))
	{
		DebugMsg("failed to build UploadBuffer", DebugFlag::WARNING);
	}

	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = _data;
	subResourceData.RowPitch = _byteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);

	_list->ResourceBarrier(1, &barrier);

	UpdateSubresources<1>(_list, defaultBuffer, uploadBuffer, 0, 0, 1, &subResourceData);

	CD3DX12_RESOURCE_BARRIER svalue = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);

	_list->ResourceBarrier(1, &svalue);

	return defaultBuffer;
}
