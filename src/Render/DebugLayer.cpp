#include "pch.h"
#include "DebugLayer.h"

DebugLayer::DebugLayer()
{
#if defined(DEBUG) || defined(_DEBUG)
{
		ID3D12Debug* debugController;
}
#endif
}

DebugLayer::~DebugLayer()
{
	m_debugController->Release();
}
