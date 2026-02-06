#include "pch.h"
#include "RenderDevice.h"
#include "DebugLayer.h"

RenderDevice::RenderDevice()
{
	m_debugLayer = nullptr;
    m_adapter = nullptr;
    m_device = nullptr;
    m_factory = nullptr;
}

int RenderDevice::InitWindow(int _width, int _height,const wchar_t* _title, WNDCLASS _windowsClass, HINSTANCE _hInstance, int _cmdShow)
{
    HWND hwnd = CreateWindowEx(
        0,
        _windowsClass.lpszClassName,
        L"ENETRE",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height,
        nullptr, nullptr, _hInstance, nullptr);
    if (!hwnd)
        return 0;
    ShowWindow(hwnd, _cmdShow);
    UpdateWindow(hwnd);
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return static_cast<int>(msg.wParam);
}

bool RenderDevice::InitDebugLayer()
{
	m_debugLayer = new DebugLayer();
    if (m_debugLayer->GDebugController() != nullptr)
    {
        return true;
    }
    return false;
}

bool RenderDevice::InitDXGIFactory()
{
    if(SUCCEEDED(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,IID_PPV_ARGS(&m_factory))));
        return true;
    return false;
}

void RenderDevice::InitAdapter()
{
    for (UINT adapterIndex = 0; m_factory->EnumAdapters1(adapterIndex, &m_adapter) != DXGI_ERROR_NOT_FOUND; ++adapterIndex)
    {
		DXGI_ADAPTER_DESC1 desc;
        m_adapter->GetDesc1(&desc);
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
            continue;
        if (SUCCEEDED(D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			break;

    }
}

void RenderDevice::InitD3D12()
{
     // 1. Définir la taille du vertex buffer (exemple : 1024 octets)
    const UINT64 vertexBufferSize = 1024;

    // 2. Créer la description de ressource
    auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(vertexBufferSize);

    // 3. Propriétés du Heap (Où la ressource habite)
    auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

    // 4. Création de la ressource
    HRESULT hr = m_device->CreateCommittedResource(
        &heapProps,
        D3D12_HEAP_FLAG_NONE,
        &resourceDesc,
        D3D12_RESOURCE_STATE_COPY_DEST, // On commence en état "Destination de copie"
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)
    );
}

bool RenderDevice::InitDevice()
{
	HRESULT hr = D3D12CreateDevice(m_adapter, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
    if (FAILED(hr))
        return false;
	return true;
}

void RenderDevice::Update()
{
}

void RenderDevice::Draw()
{
}



