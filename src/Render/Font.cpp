#include "pch.h"
#include "Font.h"
#include "DDSTextureLoader12.h"
#include "PipelineStateObject2D.h"
#include "SwapChain.h"
#include "d3dx12.h"
#include <fstream>

Font::~Font()
{
	if (m_texture) m_texture->Release();
	if (m_textureUpload) m_textureUpload->Release();
	if (m_vGPUBuffer) m_vGPUBuffer->Release();
	if (m_iGPUBuffer) m_iGPUBuffer->Release();
	if (m_vUpload) m_vUpload->Release();
	if (m_iUpload) m_iUpload->Release();
}

bool Font::Init(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList, WString _texturePath, int32 _rows, int32 _cols)
{
	m_rows = _rows;
	m_cols = _cols;
	m_charWidth = 1.0f / static_cast<float32>(_cols);
	m_charHeight = 1.0f / static_cast<float32>(_rows);

	std::unique_ptr<uint8_t[]> ddsData;
	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	HRESULT hr = DirectX::LoadDDSTextureFromFile(
		_device,
		_texturePath.c_str(),
		&m_texture,
		ddsData,
		subresources
	);
	if (FAILED(hr))
	{
		DebugMsg(L"Failed to load font texture: " + _texturePath, DebugFlag::ERROR_);
		return false;
	}
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_texture, 0, static_cast<UINT>(subresources.size()));

	auto uploadHeapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto uploadBufferDesc = CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize);

	hr = _device->CreateCommittedResource(
		&uploadHeapProps,
		D3D12_HEAP_FLAG_NONE,
		&uploadBufferDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_textureUpload));

	if (FAILED(hr))
	{
		DebugMsg(L"Failed to create texture upload buffer", DebugFlag::ERROR_);
		return false;
	}
	UpdateSubresources(_cmdList, m_texture, m_textureUpload, 0, 0, 
		static_cast<UINT>(subresources.size()), subresources.data());
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_texture,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
	_cmdList->ResourceBarrier(1, &barrier);
	m_vertices = {
		{{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
		{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
		{{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
		{{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}
	};
	m_indices = { 0, 1, 2, 0, 2, 3 };

	InitDefaultASCII();

	return true;
}

void Font::InitDefaultASCII()
{
	for (int32 i = 0; i < m_rows * m_cols; ++i)
	{
		char c = static_cast<char>(32 + i);
		if (c > 126) break;

		int32 row = i / m_cols;
		int32 col = i % m_cols;

		float32 u = static_cast<float32>(col) * m_charWidth;
		float32 v = static_cast<float32>(row) * m_charHeight;

		AddCharacter(c, u, v, m_charWidth, m_charHeight, 0);
	}
}

void Font::AddCharacter(char _character, float32 _u, float32 _v, float32 _width, float32 _height, int32 _advanceX)
{
	charDefinition def;
	def.u = _u;
	def.v = _v;
	def.width = _width;
	def.height = _height;
	def.aspectRatio = _width / _height;
	def.advanceX = _advanceX;
	m_alphabet[_character] = def;
}

bool Font::Upload(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	if (m_isUploaded) return true;

	UploadVertex(_rd, _list);
	UploadIndex(_rd, _list);
	m_isUploaded = true;

	return false;
}

void Font::Bind(ID3D12GraphicsCommandList* _list)
{
	D3D12_VERTEX_BUFFER_VIEW vbv;
	vbv.BufferLocation = m_vGPUBuffer->GetGPUVirtualAddress();
	vbv.StrideInBytes = m_vertexByteStride;
	vbv.SizeInBytes = m_vertexBufferByteSize;

	D3D12_INDEX_BUFFER_VIEW ibv;
	ibv.BufferLocation = m_iGPUBuffer->GetGPUVirtualAddress();
	ibv.Format = IndexFormat;
	ibv.SizeInBytes = IndexBufferByteSize;

	_list->IASetVertexBuffers(0, 1, &vbv);
	_list->IASetIndexBuffer(&ibv);
}

//void Font::DrawString(ID3D12GraphicsCommandList* _cmdList, PipelineStateObject2D* _pso, SwapChain* _swapChain,
//	ID3D12DescriptorHeap* _srvHeap, String _text, float32 _x, float32 _y, float32 _size)
//{
//	_cmdList->SetPipelineState(_pso->GPipelineState());
//	_cmdList->SetGraphicsRootSignature(_pso->GRootSig());
//	_cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	ID3D12DescriptorHeap* heaps[] = { _srvHeap };
//	_cmdList->SetDescriptorHeaps(1, heaps);
//	_cmdList->SetGraphicsRootDescriptorTable(1, _srvHeap->GetGPUDescriptorHandleForHeapStart());
//
//	DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicOffCenterLH(
//		0.0f, _swapChain->GViewport()->Width,
//		_swapChain->GViewport()->Height, 0.0f,
//		0.0f, 1.0f);
//
//	Bind(_cmdList);
//
//	float32 cursorX = _x;
//
//	for (char c : _text)
//	{
//		if (c == ' ')
//		{
//			cursorX += _size * 0.5f;
//			continue;
//		}
//
//		if (m_alphabet.find(c) == m_alphabet.end())
//			continue;
//
//		charDefinition& def = m_alphabet[c];
//
//		float32 charWidth = _size * def.aspectRatio;
//		float32 charHeight = _size;
//
//		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(charWidth, charHeight, 1.0f);
//		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(cursorX, _y, 0.0f);
//		DirectX::XMMATRIX world = scale * translation;
//		DirectX::XMMATRIX worldViewProj = world * ortho;
//
//		FontConstantBuffer cbData;
//		DirectX::XMStoreFloat4x4(&cbData.gWorld, DirectX::XMMatrixTranspose(worldViewProj));
//		cbData.uvOffset = { def.u, def.v, def.width, def.height };
//
//		m_fontCB->CopyData(0, cbData);
//		_cmdList->SetGraphicsRootConstantBufferView(0, m_fontCB->GetAddress());
//
//		_cmdList->DrawIndexedInstanced(uint32 (m_indices.size()), 1, 0, 0, 0);
//
//		cursorX += charWidth + (def.advanceX * _size);
//	}
//}

void Font::UploadVertex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	const UINT vbByteSize = static_cast<UINT>(m_vertices.size()) * sizeof(fontVertex);
	D3DCreateBlob(vbByteSize, &m_vBuffer);
	memcpy(m_vBuffer->GetBufferPointer(), m_vertices.data(), vbByteSize);

	m_vGPUBuffer = CreateDefaultBuffer(_rd, _list, m_vertices.data(), vbByteSize, m_vUpload);

	m_vertexByteStride = sizeof(fontVertex);
	m_vertexBufferByteSize = vbByteSize;
}

void Font::UploadIndex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list)
{
	const UINT ibByteSize = static_cast<UINT>(m_indices.size()) * sizeof(uint32);
	D3DCreateBlob(ibByteSize, &m_iBuffer);
	memcpy(m_iBuffer->GetBufferPointer(), m_indices.data(), ibByteSize);

	m_iGPUBuffer = CreateDefaultBuffer(_rd, _list, m_indices.data(), ibByteSize, m_iUpload);

	IndexFormat = DXGI_FORMAT_R32_UINT;
	IndexBufferByteSize = ibByteSize;
}

ID3D12Resource* Font::CreateDefaultBuffer(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list,
	const void* _data, UINT64 _byteSize, ID3D12Resource*& _uploadBuffer)
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

	auto uploadProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(_byteSize);

	hr = _rd->CreateCommittedResource(&uploadProp, D3D12_HEAP_FLAG_NONE, &resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&_uploadBuffer));
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

	UpdateSubresources<1>(_list, defaultBuffer, _uploadBuffer, 0, 0, 1, &subResourceData);

	auto barrier2 = CD3DX12_RESOURCE_BARRIER::Transition(defaultBuffer,
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	_list->ResourceBarrier(1, &barrier2);

	return defaultBuffer;
}

bool Font::Load(Font& _font, WString _texturePath, WString _dataPath)
{
	std::wifstream file(_dataPath);
	if (!file.is_open())
	{
		Log(true,"Failed to open font data file");
		return 1;
	}

	float32 texWidth = 256.0f;
	float32 texHeight = 256.0f;

	WString line;
	while (std::getline(file, line))
	{
		if (line.substr(0, 5) == L"char ")
		{
			int id, x, y, width, height, xadvance;
			if (swscanf_s(line.c_str(), L"char id=%d x=%d y=%d width=%d height=%d xoffset=%*d yoffset=%*d xadvance=%d",
				&id, &x, &y, &width, &height, &xadvance) == 6)
			{
				float u = (float)x / texWidth;
				float v = (float)y / texHeight;
				float wUV = (float)width / texWidth;
				float hUV = (float)height / texHeight;
				_font.AddCharacter(id, u, v, wUV, hUV, xadvance);
			}
		}
		else if (line.substr(0, 7) == L"common ")
		{
			swscanf_s(line.c_str(), L"common lineHeight=%*d base=%*d scaleW=%f scaleH=%f", &texWidth, &texHeight);
		}
	}
	file.close();
	Log(false, "Font data loaded successfully");
	return 0;
}