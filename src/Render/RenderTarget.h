#pragma once

class Descriptor;

class RenderTarget
{
public:
	RenderTarget() = default ;
	~RenderTarget();
	ID3D12Resource* GCurrentBackBuffer() const { return m_backbuffer[m_CurrentBuffer]; }
	ID3D12Resource* GCurrentBackBuffer(int _index) const { return m_backbuffer[_index]; }

	ID3D12Resource* GDepthStencilBuffer() const { return m_DepthStencilBuffer; }
	void SDepthStencilBuffer(ID3D12Resource* _buffer) { m_DepthStencilBuffer = _buffer; }

	void SwapBuffers() { m_CurrentBuffer = (m_CurrentBuffer + 1) % BufferCount; }
	static const int GCBufferCount() { return BufferCount; }
	void SCurrentBackBuffer(bool _count,ID3D12Resource* _buffer) { m_backbuffer[_count] = _buffer; }
	void SCurrentBackBuffer(bool _count) { m_CurrentBuffer = _count; }
	D3D12_CPU_DESCRIPTOR_HANDLE GCurrentBackBufferView(ID3D12DescriptorHeap* _rtvheap, UINT _rtvsize) const;
private:
	static const int BufferCount = 2;
	ID3D12Resource* m_backbuffer[BufferCount];
	ID3D12Resource* m_DepthStencilBuffer;

	bool m_CurrentBuffer = 0;
};

