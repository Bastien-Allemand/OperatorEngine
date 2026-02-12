#pragma once

class PipelineStateObject2D
{
public:
	PipelineStateObject2D() = default;
	bool Init(ID3D12Device* _device);
	ID3D12PipelineState* GPipelineState() { return m_pipelineState; }
	ID3D12RootSignature* GRootSig() { return m_rootSig; }

private:
	ID3D12PipelineState* m_pipelineState = nullptr;
	ID3D12RootSignature* m_rootSig = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;
	ID3DBlob* m_vertexShader = nullptr;
	ID3DBlob* m_pixelShader = nullptr;

	bool InitRootSig(ID3D12Device* _device);
	bool InitVs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename);
	bool InitPs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename);
	bool InitInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc);
};

