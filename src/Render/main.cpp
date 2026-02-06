#include "pch.h"
#include "main.h"
#include "RenderDevice.h"

struct ImageConstants
{
	DirectX::XMFLOAT4X4 WorldViewProj = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
};

template<typename T>
class UploadBuffer
{
public:
	UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) :
		mIsConstantBuffer(isConstantBuffer)
	{
		mElementByteSize = sizeof(T);
		if (isConstantBuffer)
			mElementByteSize = (sizeof(T) + 255) & ~255;

		device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * elementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&mUploadBuffer));

		mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));
	}
private:
	ID3D12Resource* mUploadBuffer;
	UINT mElementByteSize;
	bool mIsConstantBuffer;
	BYTE* mMappedData;
};

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	const int width = 800;
	const int height = 600;
	const wchar_t* title = L"ENETRE";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"SampleWindowClass";
	RegisterClass(&wc);

	RenderDevice renderDevice;

	ID3D12CommandQueue* mCommandQueue = nullptr;

	ID3D12CommandList* mCommandList = nullptr;

	IDXGISwapChain* mSwapChain = nullptr;

	ID3DBlob* mVertexBuffer = nullptr;

	ID3D10Blob* mIndexBuffer = nullptr;

	ID3D12Device* md3dDevice = nullptr;
	HRESULT hr = D3D12CreateDevice(
		nullptr,
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&md3dDevice)
	);	

	ID3D12CommandAllocator* mCommandAllocator = nullptr;

	ID3DBlob* mvsByteCode = nullptr;
	ID3DBlob* mpsByteCode = nullptr;
	ID3D12RootSignature* mRootSignature = nullptr;
	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;
	ID3D12PipelineState* mPipelineState = nullptr;

	//

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
	psoDesc.pRootSignature = mRootSignature;
	psoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	psoDesc.VS =
	{
		reinterpret_cast<BYTE*>(mvsByteCode->GetBufferPointer()),
		mvsByteCode->GetBufferSize()
	};
	psoDesc.PS =
	{
		reinterpret_cast<BYTE*>(mpsByteCode->GetBufferPointer()),
		mpsByteCode->GetBufferSize()
	};
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);

	//

	ID3D12DescriptorHeap* mCbvHeap = nullptr;
    D3D12_DESCRIPTOR_HEAP_DESC cbvHeapDesc;
    cbvHeapDesc.NumDescriptors = 1;
    cbvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    cbvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    cbvHeapDesc.NodeMask = 0;
	md3dDevice->CreateDescriptorHeap(&cbvHeapDesc, IID_PPV_ARGS(&mCbvHeap));

	UploadBuffer<ImageConstants> mConstantBuffer(md3dDevice, 1, true);

    ID3D12Resource** texture;
	texture = new ID3D12Resource*;
    std::unique_ptr<uint8_t[]> ddsData;
    std::vector<D3D12_SUBRESOURCE_DATA> subresources;

    HRESULT image = DirectX::LoadDDSTextureFromFile(
		md3dDevice,
        L"../../../../res/Render/test.dds",
        texture,
        ddsData,
        subresources
    );

	CD3DX12_ROOT_PARAMETER slotRootParameter[1];
	slotRootParameter[0].InitAsConstantBufferView(0);
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, slotRootParameter, 0, nullptr,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	ID3DBlob* serializedRootSig = nullptr;
	ID3DBlob* errorBlob = nullptr;
	HRESULT param = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializedRootSig, &errorBlob);

	if (errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}

	ID3DBlob* shader = nullptr;
	ID3DBlob* error = nullptr;

	HRESULT shaderCompile = D3DCompileFromFile(
		L"../../../../res/Render/test.hlsl",
		nullptr,
		nullptr,
		"main",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shader,
		&error
	);

	mCommandAllocator->Reset();
	//mCommandList->Reset(mCommandAllocator, mPipelineState);

	renderDevice.SetDevice(md3dDevice);

	return renderDevice.InitWindow(width, height, title, wc, hInstance, cmdShow);
}