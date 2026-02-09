#include "pch.h"
#include "main.h"
#include "ImageLoader.h"

// ============================================
// Structures
// ============================================
struct Vertex2D
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT2 TexCoord;
};

// ============================================
// Données globales du quad
// ============================================
Vertex2D g_quadVertices[] =
{
    { { -0.5f,  0.5f, 0.0f }, { 0.0f, 0.0f } },
    { {  0.5f,  0.5f, 0.0f }, { 1.0f, 0.0f } },
    { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f } },
    { { -0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f } },
};

UINT16 g_quadIndices[] = { 0, 1, 2, 0, 2, 3 };

// ============================================
// Variables globales D3D12
// ============================================
const UINT FrameCount = 2;

ID3D12Device* g_device = nullptr;
ID3D12CommandQueue* g_commandQueue = nullptr;
IDXGISwapChain3* g_swapChain = nullptr;
ID3D12DescriptorHeap* g_rtvHeap = nullptr;
ID3D12DescriptorHeap* g_srvHeap = nullptr;
ID3D12Resource* g_renderTargets[FrameCount] = {};
ID3D12CommandAllocator* g_commandAllocator = nullptr;
ID3D12GraphicsCommandList* g_commandList = nullptr;
ID3D12PipelineState* g_pipelineState = nullptr;
ID3D12RootSignature* g_rootSignature = nullptr;
ID3D12Resource* g_vertexBuffer = nullptr;
ID3D12Resource* g_indexBuffer = nullptr;
D3D12_VERTEX_BUFFER_VIEW g_vertexBufferView = {};
D3D12_INDEX_BUFFER_VIEW g_indexBufferView = {};

ID3D12Fence* g_fence = nullptr;
UINT64 g_fenceValue = 0;
HANDLE g_fenceEvent = nullptr;

UINT g_frameIndex = 0;
UINT g_rtvDescriptorSize = 0;
UINT g_srvDescriptorSize = 0;

D3D12_VIEWPORT g_viewport = {};
D3D12_RECT g_scissorRect = {};

HWND g_hwnd = nullptr;

// ImageLoader et Image
ImageLoader g_imageLoader;
Image g_testImage;

// ============================================
// Fonctions
// ============================================
void WaitForPreviousFrame()
{
    const UINT64 fenceVal = g_fenceValue;
    g_commandQueue->Signal(g_fence, fenceVal);
    g_fenceValue++;

    if (g_fence->GetCompletedValue() < fenceVal)
    {
        g_fence->SetEventOnCompletion(fenceVal, g_fenceEvent);
        WaitForSingleObject(g_fenceEvent, INFINITE);
    }

    g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();
}

void Render()
{
    g_commandAllocator->Reset();
    g_commandList->Reset(g_commandAllocator, g_pipelineState);

    g_commandList->SetGraphicsRootSignature(g_rootSignature);
    g_commandList->RSSetViewports(1, &g_viewport);
    g_commandList->RSSetScissorRects(1, &g_scissorRect);

    // Bind texture via ImageLoader
    g_imageLoader.BindForDraw(g_commandList, 0, g_testImage);

    // Transition render target vers RENDER_TARGET
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        g_renderTargets[g_frameIndex],
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    g_commandList->ResourceBarrier(1, &barrier);

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(
        g_rtvHeap->GetCPUDescriptorHandleForHeapStart(),
        g_frameIndex, g_rtvDescriptorSize);

    g_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    const float clearColor[] = { 0.2f, 0.2f, 0.4f, 1.0f };
    g_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // Dessiner le quad
    g_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    g_commandList->IASetVertexBuffers(0, 1, &g_vertexBufferView);
    g_commandList->IASetIndexBuffer(&g_indexBufferView);
    g_commandList->DrawIndexedInstanced(6, 1, 0, 0, 0);

    // Transition render target vers PRESENT
    barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        g_renderTargets[g_frameIndex],
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    g_commandList->ResourceBarrier(1, &barrier);

    g_commandList->Close();

    ID3D12CommandList* cmdLists[] = { g_commandList };
    g_commandQueue->ExecuteCommandLists(1, cmdLists);

    g_swapChain->Present(1, 0);

    WaitForPreviousFrame();
}

bool InitD3D12(HWND hwnd, int width, int height)
{
#ifdef _DEBUG
    ID3D12Debug* debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->Release();
    }
#endif

    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory))))
        return false;

    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&g_device))))
        return false;

    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(g_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&g_commandQueue))))
        return false;

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = FrameCount;
    swapChainDesc.Width = width;
    swapChainDesc.Height = height;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain1 = nullptr;
    factory->CreateSwapChainForHwnd(g_commandQueue, hwnd, &swapChainDesc, nullptr, nullptr, &swapChain1);
    swapChain1->QueryInterface(IID_PPV_ARGS(&g_swapChain));
    swapChain1->Release();
    factory->Release();

    g_frameIndex = g_swapChain->GetCurrentBackBufferIndex();

    // RTV Descriptor Heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = FrameCount;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    g_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&g_rtvHeap));
    g_rtvDescriptorSize = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    // Créer RTVs
    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(g_rtvHeap->GetCPUDescriptorHandleForHeapStart());
    for (UINT i = 0; i < FrameCount; i++)
    {
        g_swapChain->GetBuffer(i, IID_PPV_ARGS(&g_renderTargets[i]));
        g_device->CreateRenderTargetView(g_renderTargets[i], nullptr, rtvHandle);
        rtvHandle.Offset(1, g_rtvDescriptorSize);
    }

    // SRV Descriptor Heap pour les textures
    D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
    srvHeapDesc.NumDescriptors = 16; // Supporte jusqu'à 16 textures
    srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    g_device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&g_srvHeap));
    g_srvDescriptorSize = g_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    g_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&g_commandAllocator));

    g_viewport = { 0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f };
    g_scissorRect = { 0, 0, width, height };

    g_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&g_fence));
    g_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

    // Initialiser ImageLoader
    g_imageLoader.Initialize(g_device, g_commandQueue, g_srvHeap, g_srvDescriptorSize);

    return true;
}

bool LoadAssets()
{
    // ===== Root Signature =====
    CD3DX12_DESCRIPTOR_RANGE texTable;
    texTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

    CD3DX12_ROOT_PARAMETER rootParam;
    rootParam.InitAsDescriptorTable(1, &texTable, D3D12_SHADER_VISIBILITY_PIXEL);

    CD3DX12_STATIC_SAMPLER_DESC sampler(0, D3D12_FILTER_MIN_MAG_MIP_LINEAR);

    CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(1, &rootParam, 1, &sampler,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ID3DBlob* signature = nullptr;
    ID3DBlob* error = nullptr;
    D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
    g_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&g_rootSignature));
    if (signature) signature->Release();

    // ===== Compiler Shaders =====
    ID3DBlob* vertexShader = nullptr;
    ID3DBlob* pixelShader = nullptr;
    ID3DBlob* errorBlob = nullptr;

    HRESULT hr = D3DCompileFromFile(L"../../res/Shader/test.hlsl", nullptr, nullptr, "VS", "vs_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &vertexShader, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "VS Error", MB_OK);
        return false;
    }

    hr = D3DCompileFromFile(L"../../res/Shader/test.hlsl", nullptr, nullptr, "PS", "ps_5_0",
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelShader, &errorBlob);
    if (FAILED(hr))
    {
        if (errorBlob) MessageBoxA(nullptr, (char*)errorBlob->GetBufferPointer(), "PS Error", MB_OK);
        return false;
    }

    // ===== Input Layout =====
    D3D12_INPUT_ELEMENT_DESC inputLayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    // ===== Pipeline State =====
    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.pRootSignature = g_rootSignature;
    psoDesc.VS = { vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
    psoDesc.PS = { pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.InputLayout = { inputLayout, _countof(inputLayout) };
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;

    g_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&g_pipelineState));

    // ===== Command List =====
    g_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, g_commandAllocator, g_pipelineState, IID_PPV_ARGS(&g_commandList));
    g_commandList->Close();

    // ===== Vertex Buffer =====
    const UINT vbSize = sizeof(g_quadVertices);
    CD3DX12_HEAP_PROPERTIES heapProps(D3D12_HEAP_TYPE_UPLOAD);
    CD3DX12_RESOURCE_DESC bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(vbSize);
    g_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&g_vertexBuffer));

    void* vbData = nullptr;
    g_vertexBuffer->Map(0, nullptr, &vbData);
    memcpy(vbData, g_quadVertices, vbSize);
    g_vertexBuffer->Unmap(0, nullptr);

    g_vertexBufferView.BufferLocation = g_vertexBuffer->GetGPUVirtualAddress();
    g_vertexBufferView.StrideInBytes = sizeof(Vertex2D);
    g_vertexBufferView.SizeInBytes = vbSize;

    // ===== Index Buffer =====
    const UINT ibSize = sizeof(g_quadIndices);
    bufferDesc = CD3DX12_RESOURCE_DESC::Buffer(ibSize);
    g_device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &bufferDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&g_indexBuffer));

    void* ibData = nullptr;
    g_indexBuffer->Map(0, nullptr, &ibData);
    memcpy(ibData, g_quadIndices, ibSize);
    g_indexBuffer->Unmap(0, nullptr);

    g_indexBufferView.BufferLocation = g_indexBuffer->GetGPUVirtualAddress();
    g_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
    g_indexBufferView.SizeInBytes = ibSize;

    // ===== Charger texture via ImageLoader =====
    if (!g_imageLoader.LoadFromFile(L"../../res/Render/test.dds", g_testImage))
    {
        MessageBoxA(nullptr, "Échec chargement texture", "Erreur", MB_OK);
        return false;
    }

    return true;
}

void Cleanup()
{
    WaitForPreviousFrame();

    // Libérer l'image via ImageLoader
    g_imageLoader.ReleaseImage(g_testImage);

    // Libérer les ressources D3D12
    if (g_vertexBuffer) g_vertexBuffer->Release();
    if (g_indexBuffer) g_indexBuffer->Release();
    if (g_pipelineState) g_pipelineState->Release();
    if (g_rootSignature) g_rootSignature->Release();
    if (g_commandList) g_commandList->Release();
    if (g_commandAllocator) g_commandAllocator->Release();
    for (UINT i = 0; i < FrameCount; i++)
    {
        if (g_renderTargets[i]) g_renderTargets[i]->Release();
    }
    if (g_srvHeap) g_srvHeap->Release();
    if (g_rtvHeap) g_rtvHeap->Release();
    if (g_fence) g_fence->Release();
    if (g_swapChain) g_swapChain->Release();
    if (g_commandQueue) g_commandQueue->Release();
    if (g_device) g_device->Release();

    CloseHandle(g_fenceEvent);
}

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

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"D3D12WindowClass";
    RegisterClass(&wc);

    g_hwnd = CreateWindowEx(0, wc.lpszClassName, L"ENETRE - Texture 2D",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height,
        nullptr, nullptr, hInstance, nullptr);

    if (!g_hwnd)
        return 0;

    if (!InitD3D12(g_hwnd, width, height))
    {
        MessageBoxA(nullptr, "Échec initialisation D3D12", "Erreur", MB_OK);
        return 0;
    }

    if (!LoadAssets())
    {
        MessageBoxA(nullptr, "Échec chargement des assets", "Erreur", MB_OK);
        return 0;
    }

    ShowWindow(g_hwnd, cmdShow);
    UpdateWindow(g_hwnd);

    MSG msg = {};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            Render();
        }
    }

    Cleanup();

    return (int)msg.wParam;
}