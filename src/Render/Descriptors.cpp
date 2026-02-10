#include "pch.h"
#include "Descriptors.h"

bool Descriptors::InitRTV(ID3D12Device* _device)
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 2; // One for each buffer
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = _device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr))
	{
		std::cout << "Failed to create RTV descriptor heap" << std::endl;
		return 1;
	}
	hr = m_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	if (FAILED(hr))
	{
		std::cout << "Failed to get RTV descriptor size" << std::endl;
		return 1;
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	return 0;
}

bool Descriptors::InitDSV(ID3D12Device* _device)
{
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 2; // One for each buffer
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HRESULT hr = _device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr))
	{
		std::cout << "Failed to create DSV descriptor heap" << std::endl;
		return 1;
	}
	hr = m_rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	if (FAILED(hr))
	{
		std::cout << "Failed to get DSV descriptor size" << std::endl;
		return 1;
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	return 0;
}
