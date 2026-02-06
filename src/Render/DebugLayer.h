#pragma once
class DebugLayer
{
	public:
	DebugLayer();
	~DebugLayer();
	ID3D12Debug* GDebugController() const { return m_debugController; }
private:
	ID3D12Debug* m_debugController;
};

