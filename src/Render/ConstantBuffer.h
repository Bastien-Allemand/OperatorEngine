#pragma once
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D12Device* _device, ID3D12GraphicsCommandList* _commandList);

	void AddData(void* _data, size_t _size);

	ID3D12Resource* GVertexBuffer() const { return m_vertexBuffer; }
	ID3D12Resource* GIndexBuffer() const { return m_indexBuffer; }

private:

	void* m_data;
	ID3D12Resource* m_vertexBuffer;
	ID3D12Resource* m_indexBuffer;
	ID3D12Resource* m_vertexUpload;
};

