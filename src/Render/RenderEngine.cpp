#include "pch.h"
#include "RenderEngine.h"
#include "d3dx12.h"

#include "Mesh.h"
#include "Fence.h"
#include "Factory.h"
#include "Geometry.h"
#include "SwapChain.h"
#include "Descriptors.h"
#include "RenderDevice.h"
#include "RenderTarget.h"
#include "DirectXColors.h"
#include "CommandContext.h"
#include "PipelineStateObject.h"
#include "ConstantBuffer.h"


RenderEngine::~RenderEngine()
{
	if (m_renderDevice && m_renderDevice->GDevice())
	{
		FlushCommandQueue();
	}

	delete m_quadMesh;
	if (m_queue) m_queue->Release();

	delete m_factory;
	delete m_renderDevice;
	delete m_commandContext;
	delete m_pipelineStateObject;
	delete m_swapChain;
	delete m_fence;
	delete m_desc;
	delete m_renderTarget;
}

bool RenderEngine::Init(int _width, int _height, HWND _handle)
{
	m_factory = new Factory();
	m_renderDevice = new RenderDevice();
	m_commandContext = new CommandContext();
	m_pipelineStateObject = new PipelineStateObject();
	m_swapChain = new SwapChain();
	m_fence = new Fence();
	m_desc = new Descriptors();
	m_renderTarget = new RenderTarget();
	m_sceneCB = new ConstantBuffer<SceneConstantBuffer>();

	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_world, world);
	DirectX::XMStoreFloat4x4(&m_view, world);
	DirectX::XMStoreFloat4x4(&m_proj, world);
	//init factory
	if (m_factory->InitFactory())
	{
		std::cout << "Failed to initialize factory" << std::endl;
		return 1;
	}

	//init render device
	if (m_renderDevice->Init(m_factory))
	{
		std::cout << "Failed to initialize render device" << std::endl;
		return 1;
	}

	//init command context
	if (m_commandContext->Init(m_renderDevice))
	{
		std::cout << "Failed to initialize command context" << std::endl;
		return 1;
	}

	//init queue
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr = m_renderDevice->GDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
	{
		std::cout << "Command Queue creation failed" << std::endl;
		return 1;
	}

	//init swapchain
	if (m_swapChain->Init(_width, _height, _handle,m_factory->GFactory(),m_queue))
	{
		std::cout << "Failed to initialize swapchain" << std::endl;
		return 1;
	}

	//init descriptor heap for render target views
	if (m_desc->InitRTV(m_renderDevice->GDevice()))
	{
		std::cout << "Failed to Init RTV" << std::endl;
		return 1;
	}
	if (m_desc->InitDSV(m_renderDevice->GDevice()))
	{
		std::cout << "Failed to Init DSV" << std::endl;
		return 1;
	}
	if (m_desc->InitCBV(m_renderDevice->GDevice()))
	{
		DebugMsg("Failed to Init CBV", DebugFlag::WARNING);
	}
	hr = m_pipelineStateObject->Init(m_renderDevice->GDevice());
	if (FAILED(hr))
	{
		std::cout << "Failed to Init PSO" << std::endl;
		return 1;
	}

	if (m_fence->Init(m_renderDevice->GDevice()))
	{
		std::cout << "Failed to initialize fence" << std::endl;
		return 1;
	}



	Resize(_width, _height);

	if (m_sceneCB->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 0))
	{
		DebugMsg("Failed to initialize scene constant buffer", DebugFlag::WARNING);
	}

	m_commandContext->GCommandAllocator()->Reset();
	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), nullptr);

	Geometry geo;
	geo.BuildBox();
	m_quadMesh = new Mesh(geo);
	m_quadMesh->Upload(m_renderDevice->GDevice(), m_commandContext->GCommandList());



	m_commandContext->CloseAndExecute(m_queue);

	FlushCommandQueue();



}

void RenderEngine::Update()
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);

	DirectX::XMStoreFloat4x4(&m_view, view);
	DirectX::XMMATRIX world = XMLoadFloat4x4(&m_world);

	world = world * DirectX::XMMatrixRotationZ(0.001f);
	world = world * DirectX::XMMatrixRotationY(0.001f);
	world = world * DirectX::XMMatrixRotationX(0.001f);
	DirectX::XMStoreFloat4x4(&m_world, world);
	DirectX::XMMATRIX proj = XMLoadFloat4x4(&m_proj);
	SceneConstantBuffer cb;
	DirectX::XMMATRIX worldViewProj = world * view * proj;
	XMStoreFloat4x4(&cb.gWorldViewProj, XMMatrixTranspose(worldViewProj));

	m_sceneCB->CopyData(0,cb);

	auto list = m_commandContext->GCommandList();
	m_commandContext->GCommandAllocator()->Reset();
	list->Reset(m_commandContext->GCommandAllocator(), nullptr);

	list->RSSetViewports(1, &m_swapChain->GViewport());
	list->RSSetScissorRects(1, &m_swapChain->GScissorRect());

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	list->ResourceBarrier(1, &barrier);

	list->ClearRenderTargetView(m_renderTarget->GCurrentBackBufferView(m_desc->GrtvHeap(), m_desc->GCrtvSize()), DirectX::Colors::Aquamarine, 0, nullptr);
	list->ClearDepthStencilView(m_desc->GdsvHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTarget->GCurrentBackBufferView(m_desc->GrtvHeap(), m_desc->GCrtvSize());
	auto dsvHandle = m_desc->GdsvHandle();
	list->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_desc->GcbvHeap()};
	list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	list->SetGraphicsRootSignature(m_pipelineStateObject->GRootSig());

	list->SetPipelineState(m_pipelineStateObject->GPipelineState());


	//m_commandContext->GCommandList()->SetGraphicsRootDescriptorTable(0, m_sceneCB->GpuHandle());

	if (m_quadMesh)
	{
		m_quadMesh->Bind(list);
		list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		list->SetGraphicsRootConstantBufferView(0, m_sceneCB->GetAddress());
		list->DrawIndexedInstanced(m_quadMesh->DrawArgs["box"].IndexCount, 1, 0, 0, 0);
	}

	//int i = 0;
	//for (auto* mesh : m_meshes)
	//{
	//	mesh->Upload(m_renderDevice->GDevice(), m_commandContext->GCommandList()); // Propre !
	//	mesh->Bind(m_commandContext->GCommandList()); // Propre !

	//	// On dessine : 6 indices pour les 2 triangles du quad
	//	m_commandContext->GCommandList()->DrawIndexedInstanced(mesh->GetIndexCount(), 1, 0, 0, 0);
	//	
	//	m_meshes.erase(m_meshes.begin() + i);
	//	mesh = nullptr;
	//	i++;
	//}

	//D3D12_GPU_VIRTUAL_ADDRESS cbvAddress;
	//cbvAddress
	

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GCurrentBackBuffer(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	list->ResourceBarrier(1, &barrier);

	m_commandContext->CloseAndExecute(m_queue);

	HRESULT hr = m_swapChain->GSwapChain()->Present(1, 0);
	if (FAILED(hr)) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			HRESULT reason = m_renderDevice->GDevice()->GetDeviceRemovedReason();
			std::cout << "Device Removed! Reason: " << std::hex << reason << std::endl;
		}
	}
	m_renderTarget->SwapBuffers();


	FlushCommandQueue();


}

bool RenderEngine::Resize(int _width, int _height)
{
	DirectX::XMMATRIX proj = DirectX::XMMatrixPerspectiveFovLH(0.25f * DirectX::XM_PI, static_cast<float>(_width) / _height, 0.1f, 1000.0f);
	DirectX::XMStoreFloat4x4(&m_proj, proj);

	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), nullptr);
	for (int i = 0; i < m_renderTarget->GCBufferCount(); i++)
	{
		if (m_renderTarget->GCurrentBackBuffer(i) == nullptr) continue;
		m_renderTarget->GCurrentBackBuffer(i)->Release();
	}
	if (m_renderTarget->GDepthStencilBuffer() != nullptr)
		m_renderTarget->GDepthStencilBuffer()->Release();

	HRESULT hr = m_swapChain->GSwapChain()->ResizeBuffers(m_renderTarget->GCBufferCount(), _width, _height, m_swapChain->GFormat(), DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
	if(FAILED(hr))
	{
		DebugMsg("Failed to resize swapchain buffers", DebugFlag::WARNING);
		return 1;
	}

	m_renderTarget->SCurrentBackBuffer(0);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_desc->GrtvHandle());
	for (uint32 i = 0; i < m_renderTarget->GCBufferCount(); i++)
	{
		ID3D12Resource* backBuffer = nullptr;
		m_swapChain->GSwapChain()->GetBuffer(i, IID_PPV_ARGS(&backBuffer));

		// ICI : Il faut mettre à jour ton objet RenderTarget !
		m_renderTarget->SCurrentBackBuffer(i, backBuffer);

		m_renderDevice->GDevice()->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);
		rtvHandle.Offset(1, m_desc->GCrtvSize());
	}

	// Create the depth/stencil buffer and view.
	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = _width;
	depthStencilDesc.Height = _height;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	depthStencilDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear = {};
	optClear.Format = m_swapChain->depthStencilFormat;
	optClear.DepthStencil.Depth = 1.0f;
	optClear.DepthStencil.Stencil = 0;

	ID3D12Resource* depthStencilResource = nullptr;
	auto heapprop = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

	m_renderDevice->GDevice()->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear,
		IID_PPV_ARGS(&depthStencilResource));

	// MAJ du membre de classe (comme tu l'as fait pour les backbuffers)
	m_renderTarget->SDepthStencilBuffer(depthStencilResource);

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};	
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = m_swapChain->depthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;
	m_renderDevice->GDevice()->CreateDepthStencilView(m_renderTarget->GDepthStencilBuffer(), &dsvDesc, m_desc->GdsvHandle());
	auto bar = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GDepthStencilBuffer(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
	m_commandContext->GCommandList()->ResourceBarrier(1, &bar);

	m_commandContext->CloseAndExecute(m_queue);

	FlushCommandQueue();

	m_swapChain->GViewport().TopLeftX = 0;
	m_swapChain->GViewport().TopLeftY = 0;
	m_swapChain->GViewport().Width = static_cast<float>(_width);
	m_swapChain->GViewport().Height = static_cast<float>(_height);
	m_swapChain->GViewport().MinDepth = 0.0f;
	m_swapChain->GViewport().MaxDepth = 1.0f;

	m_swapChain->GScissorRect() = {0, 0, _width, _height };


}

bool RenderEngine::FlushCommandQueue()
{
	m_fence->SFenceValue(m_fence->GCFenceValue() + 1);

	HRESULT hr = m_queue->Signal(m_fence->GFence(), m_fence->GCFenceValue());
	if (FAILED(hr))
	{
		DebugMsg("Failed to signal command queue", DebugFlag::ERROR_);
	}

	if (m_fence->GFence()->GetCompletedValue() < m_fence->GCFenceValue() )
	{
		HANDLE eventHandle = CreateEventEx(nullptr, (LPCWSTR)false, false, EVENT_ALL_ACCESS);

		m_fence->GFence()->SetEventOnCompletion(m_fence->GCFenceValue(), eventHandle);

		WaitForSingleObject(eventHandle, INFINITE);
		CloseHandle(eventHandle);
	}
	return 0;
}