#pragma once
class PipelineStateObject
{
public:
	PipelineStateObject() = default;
	bool Init(ID3D12Device* _device, D3D12_GRAPHICS_PIPELINE_STATE_DESC* _psoDesc);
	bool InitVs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename);
	bool InitPs(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, const wchar_t* _filename);
	bool InitInputLayout(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc);
	ID3D12PipelineState* GPipelineState() { return m_pipelineState; }
private:
	ID3D12PipelineState* m_pipelineState = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> m_inputLayout;

	ID3DBlob* m_vertexShader = nullptr;
	ID3DBlob* m_pixelShader = nullptr;
};

