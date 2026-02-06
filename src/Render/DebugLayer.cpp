#include "pch.h"
#include "DebugLayer.h"

DebugLayer::DebugLayer()
{
#ifdef _DEBUG
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&m_debugController))))
		m_debugController->EnableDebugLayer();
#endif


}

DebugLayer::~DebugLayer()
{
	m_debugController->Release();
}
