#pragma once

#include "ConstantBuffer.h"

class PipelineStateObject2D;
class SwapChain;
struct SubmeshGeometry;

struct charDefinition
{
	float32 u;
	float32 v;
	float32 width;
	float32 height;
	float32 aspectRatio;
	int32 advanceX;
};

class Font
{
public:
	Font() = default;
	~Font();

	bool Init(ID3D12Device* _device, ID3D12GraphicsCommandList* _cmdList, WString _texturePath, int32 _rows, int32 _cols);

	void AddCharacter(char _character, float32 _u, float32 _v, float32 _width, float32 _height, int32 _advanceX);

	void InitDefaultASCII();

	bool Upload(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	void Bind(ID3D12GraphicsCommandList* _list);

	/*void DrawString(ID3D12GraphicsCommandList* _cmdList, PipelineStateObject2D* _pso, SwapChain* _swapChain,
		ID3D12DescriptorHeap* _srvHeap, String _text, float32 _x, float32 _y, float32 _size);*/

	size_t GIndexCount() const { return m_indices.size(); }
	ID3D12Resource* GTexture() { return m_texture; }
	const UnOrderedMap<int, charDefinition>& GAlphabet() const { return m_alphabet; }

	bool Load(Font& _font, WString _texturePath, WString _dataPath);

private:
	void UploadVertex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	void UploadIndex(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list);
	ID3D12Resource* CreateDefaultBuffer(ID3D12Device* _rd, ID3D12GraphicsCommandList* _list, const void* _data, UINT64 _byteSize, ID3D12Resource*& _uploadBuffer);

	ID3D12Resource* m_texture = nullptr;
	ID3D12Resource* m_textureUpload = nullptr;

	ID3DBlob* m_vBuffer = nullptr;
	ID3DBlob* m_iBuffer = nullptr;
	ID3D12Resource* m_vGPUBuffer = nullptr;
	ID3D12Resource* m_iGPUBuffer = nullptr;
	ID3D12Resource* m_vUpload = nullptr;
	ID3D12Resource* m_iUpload = nullptr;

	uint32 m_vertexByteStride = 0;
	uint32 m_vertexBufferByteSize = 0;

	DXGI_FORMAT IndexFormat = DXGI_FORMAT_R32_UINT;
	UINT IndexBufferByteSize = 0;

	int32 m_rows = 0;
	int32 m_cols = 0;
	float32 m_charWidth = 0.0f;
	float32 m_charHeight = 0.0f;

	struct fontVertex
	{
		Vector3f pos;
		Vector2f uv;
	};	

	UnOrderedMap<int, charDefinition> m_alphabet;
	Vector<fontVertex> m_vertices;
	Vector<uint32> m_indices;

	bool m_isUploaded = false;
};

