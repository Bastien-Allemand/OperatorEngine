#pragma once
class Factory
{
public:
	Factory();
	bool Init();
	~Factory();
	IDXGIFactory4* GFactory() const { return m_factory; }
	IDXGIAdapter1* GAdapter() const { return m_adapter; }
private:
	bool InitFactory();
	IDXGIFactory4* m_factory = nullptr;
	IDXGIAdapter1* m_adapter = nullptr;
};

