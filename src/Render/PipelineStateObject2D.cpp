#include "pch.h"
#include "PipelineStateObject2D.h"
#include "Utils.h"
#include <iostream>
#include <d3dx12.h>

bool PipelineStateObject2D::Init(ID3D12Device* _device)
{
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};

	InitInputLayout(&psoDesc);
	InitVs(&psoDesc, L"../../res/Render/Shader/Shader2D.txt");
	InitPs(&psoDesc, L"../../res/Render/Shader/Shader2D.txt");
	InitRootSig(_device);

	psoDesc.pRootSignature = m_rootSig;
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // Pas de culling pour le 2D

	D3D12_BLEND_DESC blendDesc = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	psoDesc.BlendState = blendDesc;

	D3D12_DEPTH_STENCIL_DESC dsDesc = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	dsDesc.DepthEnable = FALSE;
	dsDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	psoDesc.DepthStencilState = dsDesc;

	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	psoDesc.SampleDesc.Count = 1;

	HRESULT hr = _device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr))
	{
		Log(true, "Failed to create 2D pipeline state");
		return 1;
	}

	return 0;
}

bool PipelineStateObject2D::InitRootSig(ID3D12Device* _device)
{
	CD3DX12_DESCRIPTOR_RANGE srvRange;
	srvRange.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0

	CD3DX12_ROOT_PARAMETER rootParameters[2];
	rootParameters[0].InitAsConstantBufferView(0); // register b0 - FontConstantBuffer
	rootParameters[1].InitAsDescriptorTable(1, &srvRange, D3D12_SHADER_VISIBILITY_PIXEL); // t0 - Texture

	// Sampler statique
	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 1;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0; // s0
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc;
	rootSigDesc.Init(_countof(rootParameters), rootParameters, 1, &sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	if (FAILED(hr))
	{
		Log(true, "Failed to serialize 2D root signature");
		if (errorBlob)
		{
			std::cout << "Error: " << (char*)errorBlob->GetBufferPointer() << std::endl;
			errorBlob->Release();
		}
		return 1;
	}

	hr = _device->CreateRootSignature(0, serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(), IID_PPV_ARGS(&m_rootSig));

	if (FAILED(hr))
	{
		Log(true, "Failed to create 2D root signature");
		return 1;
	}

	return 0;
}

bool PipelineStateObject2D::InitVs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename)
{
	m_vertexShader = Utils::CompileShader(_filename, nullptr, "VS", "vs_5_0");

	if (m_vertexShader == nullptr)
	{
		Log(true, "2D Vertex shader compilation failed");
		return 1;
	}
	_desc->VS = { m_vertexShader->GetBufferPointer(), m_vertexShader->GetBufferSize() };
	return 0;
}

bool PipelineStateObject2D::InitPs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename)
{
	m_pixelShader = Utils::CompileShader(_filename, nullptr, "PS", "ps_5_0");

	if (m_pixelShader == nullptr)
	{
		Log(true, "2D Pixel shader compilation failed");
		return 1;
	}
	_desc->PS = { m_pixelShader->GetBufferPointer(), m_pixelShader->GetBufferSize() };
	return 0;
}

bool PipelineStateObject2D::InitInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc)
{
	m_inputLayout =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	_desc->InputLayout = { m_inputLayout.data(), (UINT)m_inputLayout.size() };
	return 0;
}
