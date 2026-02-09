#include "pch.h"
#include "PipelineStateObject.h"
#include "Utils.h"
#include <iostream>


bool PipelineStateObject::Init(ID3D12Device* _device, D3D12_GRAPHICS_PIPELINE_STATE_DESC* _psoDesc)
{
	if (m_vertexShader == nullptr)
	{
		std::cout << "Vertex shader not initialized" << std::endl;
		return false;
	}
	if (m_pixelShader == nullptr)
	{
		std::cout << "Pixel shader not initialized" << std::endl;
		return false;
	}
	if (m_inputLayout.size() == 0)
	{
		std::cout << "Input layout not initialized" << std::endl;
		return false;
	}

	HRESULT hr = _device->CreateGraphicsPipelineState(_psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr))
	{
		std::cout << "Failed to create pipeline state object" << std::endl;
		return false;
	}

	return true;
}

bool PipelineStateObject::InitVs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename)
{

	m_vertexShader = Utils::CompileShader(_filename, nullptr, "VS", "vs_5_0");

	if (m_vertexShader == nullptr)
	{
		std::cout << "Vertex shader compilation failed" << std::endl;
		return 1;
	}
	_desc->VS = { m_vertexShader->GetBufferPointer(), m_vertexShader->GetBufferSize() }; // if bug try casting to byte but it should work as is
	return 0;
}

bool PipelineStateObject::InitPs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename)
{
	m_pixelShader = Utils::CompileShader(_filename, nullptr, "PS", "ps_5_0");

	if (m_pixelShader == nullptr)
	{
		std::cout << "Pixel shader compilation failed" << std::endl;
		return 1;
	}
	_desc->PS = { m_pixelShader->GetBufferPointer(), m_pixelShader->GetBufferSize() }; // if bug try casting to byte but it should work as is
	return 0;
}

bool PipelineStateObject::InitInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc)
{
	m_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	_desc->InputLayout = { m_inputLayout.data(), (UINT)m_inputLayout.size() };

	return 0;
}
