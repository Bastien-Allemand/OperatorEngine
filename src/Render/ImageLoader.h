#pragma once

struct Image
{
    ID3D12Resource* texture = nullptr;
    ID3D12Resource* uploadHeap = nullptr;
    D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = {};
    D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = {};
    UINT width = 0;
    UINT height = 0;
    DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN;
    UINT heapIndex = 0;
    bool isLoaded = false;
};

class ImageLoader
{
public:
    ImageLoader() = default;
    ~ImageLoader();

    void Initialize(
        ID3D12Device* device,
        ID3D12CommandQueue* commandQueue,
        ID3D12DescriptorHeap* srvHeap,
        UINT descriptorSize
    );

    bool LoadFromFile(const wchar_t* filePath, Image& outImage);

    void BindForDraw(ID3D12GraphicsCommandList* commandList, UINT rootParameterIndex, const Image& image);

    void ReleaseImage(Image& image);

    void WaitForGPU();

private:
    bool UploadTexture(Image& outImage, std::vector<D3D12_SUBRESOURCE_DATA>& subresources);
    bool AllocateDescriptor(Image& outImage);
    void ExecuteUploadCommands();

    ID3D12Device* m_device = nullptr;
    ID3D12CommandQueue* m_commandQueue = nullptr;
    ID3D12DescriptorHeap* m_srvHeap = nullptr;
    UINT m_descriptorSize = 0;
    UINT m_nextDescriptorIndex = 0;

    ID3D12CommandAllocator* m_commandAllocator = nullptr;
    ID3D12GraphicsCommandList* m_commandList = nullptr;
    ID3D12Fence* m_fence = nullptr;
    HANDLE m_fenceEvent = nullptr;
    UINT64 m_fenceValue = 0;
};

