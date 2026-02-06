#include "pch.h"
#include "RenderDevice.h"
#include "DebugLayer.h"
#include "d3dx12.h"
#include "Geometry.h"
#include "Factory.h"
#include "Command.h"
#include "ConstantBuffer.h"

RenderDevice::RenderDevice()
{
}

bool RenderDevice::InitDevice()
{
   
	HRESULT hr = D3D12CreateDevice(m_factory->GAdapter(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
    if (FAILED(hr))
        return false;
	return true;
}

void RenderDevice::InitVertexViewBuffer()
{
    m_vertexBufferView.BufferLocation = m_constantBuffer->GVertexBuffer()->GetGPUVirtualAddress();
    m_vertexBufferView.StrideInBytes = sizeof(vertex);
    m_vertexBufferView.SizeInBytes = static_cast<UINT>(m_vertexBufferView.StrideInBytes * 4);
    m_indexBufferView.BufferLocation = m_constantBuffer->GVertexBuffer()->GetGPUVirtualAddress();
    m_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
    m_indexBufferView.SizeInBytes = static_cast<UINT>(sizeof(unsigned int) * 6);
}

void RenderDevice::Render()
{


}

void RenderDevice::Init()
{
	m_factory = new Factory();
	m_factory->Init();
	InitDevice();
	m_command = new Command(m_device);
	m_constantBuffer = new ConstantBuffer(m_device, m_command->GCommandList());
}
void RenderDevice::Update()
{

}

void RenderDevice::Draw()
{
    m_command->GCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_command->GCommandList()->IASetVertexBuffers(0, 1, &m_vertexBufferView);
    m_command->GCommandList()->IASetIndexBuffer(&m_indexBufferView);




}



