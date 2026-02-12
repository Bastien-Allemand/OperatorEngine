#include "pch.h"
#include "PipelineStateObject.h"
#include "Utils.h"
#include <iostream>
#include <d3dx12.h>

bool PipelineStateObject::Init(ID3D12Device* _device)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	// Input Layout,Vertex Shader, Pixel Shader
	InitInputLayout(&psoDesc);
	InitVs(&psoDesc, L"../../res/Render/Shader/DefaultShader.txt");
	InitPs(&psoDesc, L"../../res/Render/Shader/DefaultShader.txt");
	InitRootSig(_device);
	psoDesc.pRootSignature = m_rootSig;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX; // Standard
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.SampleDesc.Count = 1; // No MSAA

	HRESULT hr = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr))
	{
		std::cout << "Failed to create pipeline state object" << std::endl;
		return 1;
	}

	return 0;
}
bool PipelineStateObject::InitRootSig(ID3D12Device* _device)
{
	CD3DX12_ROOT_PARAMETER rootParameters[1];
	rootParameters[0].InitAsConstantBufferView(0); //register b0
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.Init(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	if (FAILED(hr))
	{
		std::cout << "Failed to serialize root signature" << std::endl;
		if (errorBlob)
		{
			std::cout << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			errorBlob->Release();
		}
		return 1;
	}
	hr = _device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSig));
	if (FAILED(hr))
	{
		std::cout << "Failed to create root signature" << std::endl;
		return 1;
	}
	return 0;
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
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	_desc->InputLayout = { m_inputLayout.data(), (UINT)m_inputLayout.size() };
	return true;
}
