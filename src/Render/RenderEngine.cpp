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
#include "PipelineStateObject2D.h"
#include "Font.h"


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
	delete m_pso2D;
	delete m_swapChain;
	delete m_fence;
	delete m_desc;
	delete m_renderTarget;
}

bool RenderEngine::Init(int _width, int _height, HWND _handle)
{
	HardInit();
	bool r;
	//init factory
	r = m_factory->InitFactory(); Log(r, "Initializing Factory")
	r = m_renderDevice->Init(m_factory); Log(r, "Initializing Render Device")
	r = m_commandContext->Init(m_renderDevice);	Log(r, "Initializing CommandContext")
	r = InitQueue(); Log(r, "Initializing Command Queue")
	r = m_swapChain->Init(_width, _height, _handle, m_factory->GFactory(), m_queue); Log(r, "Initializing SwapChain")
	r = m_desc->InitRTV(m_renderDevice->GDevice()); Log(r, "Initializing RTV")
	r = m_desc->InitDSV(m_renderDevice->GDevice()); Log(r, "Initializing DSV")
	r = m_desc->InitCBV(m_renderDevice->GDevice()); Log(r, "Initializing CBV")
	r = m_pipelineStateObject->Init(m_renderDevice->GDevice()); Log(r, "Initializing PSO")
	r = m_pso2D->Init(m_renderDevice->GDevice()); Log(r, "Initializing PSO 2D")
	r = m_fence->Init(m_renderDevice->GDevice()); Log(r, "Initializing Fence")
	r = Resize(_width, _height); Log(r,"Resizing")
	r = m_sceneCB->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 0); Log(r, "Initializing Scene Constant Buffer")
	r = m_lightCB->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 1); Log(r, "Initializing Light Constant Buffer")
	m_lightCB->CopyData(0, m_lightData);
	r = InitFont(); Log(r, "Initializing Font")
	return 0;
}

void RenderEngine::Update(float dt)
{
	float x = mRadius * sinf(mPhi) * cosf(mTheta);
	float z = mRadius * sinf(mPhi) * sinf(mTheta);
	float y = mRadius * cosf(mPhi);
	DirectX::XMVECTOR pos = DirectX::XMVectorSet(x, y, z, 1.0f);
	DirectX::XMVECTOR target = DirectX::XMVectorZero();
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(pos, target, up);

	DirectX::XMStoreFloat4x4(&m_view, view);
	DirectX::XMMATRIX world = DirectX::XMLoadFloat4x4(&m_world);

	world = world * DirectX::XMMatrixRotationZ(dt);
	world = world * DirectX::XMMatrixRotationY(dt);
	world = world * DirectX::XMMatrixRotationX(dt);
	DirectX::XMStoreFloat4x4(&m_world, world);
	DirectX::XMMATRIX proj = DirectX::XMLoadFloat4x4(&m_proj);
	ConstantBufferPass cb;
	DirectX::XMMATRIX viewProj = view * proj;
	DirectX::XMStoreFloat4x4(&cb.gViewProj, DirectX::XMMatrixTranspose(viewProj));
	DirectX::XMStoreFloat4x4(&cb.gWorld, DirectX::XMMatrixTranspose(world));

	m_sceneCB->CopyData(0, cb);
}

void RenderEngine::InitMesh(Mesh* _mesh)
{
	m_commandContext->GCommandAllocator()->Reset();
	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), nullptr);
	_mesh->Upload(m_renderDevice->GDevice(), m_commandContext->GCommandList());

	m_commandContext->CloseAndExecute(m_queue);
}

void RenderEngine::InitFont(Font* _font)
{
	m_commandContext->GCommandAllocator()->Reset();
	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), nullptr);
	_font->Upload(m_renderDevice->GDevice(), m_commandContext->GCommandList());

	m_commandContext->CloseAndExecute(m_queue);
}

void RenderEngine::BeginDraw()
{
	auto list = m_commandContext->GCommandList();
	m_commandContext->GCommandAllocator()->Reset();
	list->Reset(m_commandContext->GCommandAllocator(), nullptr);

	list->RSSetViewports(1, m_swapChain->GViewport());
	list->RSSetScissorRects(1, m_swapChain->GScissorRect());

	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GCurrentBackBuffer(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
	list->ResourceBarrier(1, &barrier);
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_renderTarget->GCurrentBackBufferView(m_desc->GrtvHeap(), m_desc->GCrtvSize());

	list->ClearRenderTargetView(rtvHandle, DirectX::Colors::LightSteelBlue, 0, nullptr);
	list->ClearDepthStencilView(m_desc->GdsvHandle(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);

	auto dsvHandle = m_desc->GdsvHandle();
	list->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	ID3D12DescriptorHeap* descriptorHeaps[] = { m_desc->GcbvHeap() };
	list->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	list->SetGraphicsRootSignature(m_pipelineStateObject->GRootSig());
	list->SetPipelineState(m_pipelineStateObject->GPipelineState());
	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	list->SetGraphicsRootConstantBufferView(0, m_sceneCB->GetAddress());
	list->SetGraphicsRootConstantBufferView(1, m_lightCB->GetAddress());

	/*list->SetGraphicsRootConstantBufferView(1, m_lightCB->GetAddress());	

	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->Upload(m_renderDevice->GDevice(), list);
		m_meshes[i]->Bind(list);
		list->DrawIndexedInstanced(m_meshes[i]->GetIndexCount(), 1, 0, 0, 0);
	}

	m_font->Upload(m_renderDevice->GDevice(), list);
	DrawString("He rg  er?./�%���", 50.0f, 50.0f, 70.0f);

	barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GCurrentBackBuffer(),*/
}

void RenderEngine::CloseDraw()
{
	auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(m_renderTarget->GCurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandContext->GCommandList()->ResourceBarrier(1, &barrier);

	m_commandContext->CloseAndExecute(m_queue);

	HRESULT hr = m_swapChain->GSwapChain()->Present(0, 0);
	if (FAILED(hr)) {
		if (hr == DXGI_ERROR_DEVICE_REMOVED) {
			HRESULT reason = m_renderDevice->GDevice()->GetDeviceRemovedReason();
			std::cout << "Device Removed! Reason: " << std::hex << reason << std::endl;
		}
	}
	m_renderTarget->SwapBuffers();

	FlushCommandQueue();
	m_instanceCbIndex = 0;
	m_fontCBIndex = 0;
}

void RenderEngine::Draw(Mesh* _mesh,Matrix _matrix)
{
	ConstantBufferPass cb;
	Matrix view = DirectX::XMLoadFloat4x4(&m_view);
	Matrix proj = DirectX::XMLoadFloat4x4(&m_proj);
	Matrix world = DirectX::XMLoadFloat4x4(&m_world);
	_matrix = world * _matrix;
	DirectX::XMMATRIX viewProj = view * proj;
	DirectX::XMStoreFloat4x4(&cb.gWorld, DirectX::XMMatrixTranspose(_matrix));
	DirectX::XMStoreFloat4x4(&cb.gViewProj, DirectX::XMMatrixTranspose(viewProj));
	if (m_instanceCB.size() <= m_instanceCbIndex)
	{
		m_instanceCB.push_back(new ConstantBuffer<ConstantBufferPass>());
		bool r = m_instanceCB[m_instanceCbIndex]->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 2 + m_instanceCbIndex);
		Log(r, "Initializing new Instance Constant Buffer");
	}
	m_instanceCB[m_instanceCbIndex]->CopyData(0, cb);
	auto list = m_commandContext->GCommandList();

	list->SetGraphicsRootConstantBufferView(0, m_instanceCB[m_instanceCbIndex]->GetAddress());
	_mesh->Bind(list);
	list->DrawIndexedInstanced(_mesh->GetIndexCount(), 1, 0, 0, 0);
	m_instanceCbIndex++;
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

		// ICI : Il faut mettre � jour ton objet RenderTarget !
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

	m_swapChain->GViewport()->TopLeftX = 0;
	m_swapChain->GViewport()->TopLeftY = 0;
	m_swapChain->GViewport()->Width = static_cast<float>(_width);
	m_swapChain->GViewport()->Height = static_cast<float>(_height);
	m_swapChain->GViewport()->MinDepth = 0.0f;
	m_swapChain->GViewport()->MaxDepth = 1.0f;

	m_swapChain->GScissorRect()->left = 0;
	m_swapChain->GScissorRect()->top = 0;
	m_swapChain->GScissorRect()->right = _width;
	m_swapChain->GScissorRect()->bottom = _height;

	return 0;
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

void RenderEngine::HardInit()
{
	m_factory = new Factory();
	m_renderDevice = new RenderDevice();
	m_commandContext = new CommandContext();
	m_pipelineStateObject = new PipelineStateObject();
	m_pso2D = new PipelineStateObject2D();
	m_swapChain = new SwapChain();
	m_fence = new Fence();
	m_desc = new Descriptors();
	m_renderTarget = new RenderTarget();
	m_sceneCB = new ConstantBuffer<ConstantBufferPass>();
	m_lightCB = new ConstantBuffer<LightData>();
	DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
	DirectX::XMStoreFloat4x4(&m_world, world);
	DirectX::XMStoreFloat4x4(&m_view, world);
	DirectX::XMStoreFloat4x4(&m_proj, world);

	DirectionalLight passData;
	passData.Ambient = { 0.1f, 0.1f, 0.1f, 1.0f };
	passData.Diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
	passData.Direction = { 0.577f, -0.577f, 0.577f };
	LightData data;
	data.DirLight = passData;
	data.EyePosW = { 0, 0, 0 };

	m_lightData = data;
}

bool RenderEngine::InitQueue()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	HRESULT hr = m_renderDevice->GDevice()->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_queue));
	if (FAILED(hr))
	{
		Log(true, "Failed to create command queue")
		return 1;
	}
	return 0;
}

bool RenderEngine::InitFont()
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT hr = m_renderDevice->GDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_fontSrvHeap));
	if (FAILED(hr))
	{
		Log(true, "Failed to create font SRV heap");
		return 1;
	}
	m_commandContext->GCommandAllocator()->Reset();
	m_commandContext->GCommandList()->Reset(m_commandContext->GCommandAllocator(), nullptr);

	m_font = new Font(L"../../res/Render/arial.dds", L"../../res/Render/arial.fnt");
	if (!m_font->Init(m_renderDevice->GDevice(), m_commandContext->GCommandList(),
		L"../../res/Render/arial.dds", 8, 16))
	{
		Log(true, "Failed to init font");
		return 1;
	}
	
	/*m_font->Load(*m_font, L"../../res/Render/arial.dds", L"../../res/Render/arial.fnt");*/

	m_commandContext->CloseAndExecute(m_queue);
	FlushCommandQueue();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = m_font->GTexture()->GetDesc().Format;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = m_font->GTexture()->GetDesc().MipLevels;

	m_renderDevice->GDevice()->CreateShaderResourceView(
		m_font->GTexture(),
		&srvDesc,
		m_fontSrvHeap->GetCPUDescriptorHandleForHeapStart());

	return 0;
}

void RenderEngine::DrawString(String _text, float32 _x, float32 _y, float32 _size)
{
	m_commandContext->GCommandList()->SetPipelineState(m_pso2D->GPipelineState());
	m_commandContext->GCommandList()->SetGraphicsRootSignature(m_pso2D->GRootSig());
	m_commandContext->GCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3D12DescriptorHeap* heaps[] = { m_fontSrvHeap };
	m_commandContext->GCommandList()->SetDescriptorHeaps(1, heaps);
	m_commandContext->GCommandList()->SetGraphicsRootDescriptorTable(1, m_fontSrvHeap->GetGPUDescriptorHandleForHeapStart());

	DirectX::XMMATRIX ortho = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, m_swapChain->GViewport()->Width,
		m_swapChain->GViewport()->Height, 0.0f,
		0.0f, 1.0f);
	
	m_font->Bind(m_commandContext->GCommandList());

	float32 cursorX = _x;

	for (char c : _text)
	{
		charDefinition def;

		if (m_font->GAlphabet().find(c) == m_font->GAlphabet().end())
		{
			def = m_font->GAlphabet().at('@');
		}
		else
		{
			def = m_font->GAlphabet().at(c);
		}		 

		float32 charWidth = _size * def.aspectRatio;
		float32 charHeight = _size;

		DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(charWidth, charHeight, 1.0f);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(cursorX, _y, 0.0f);
		DirectX::XMMATRIX world = scale * translation;
		DirectX::XMMATRIX worldViewProj = world * ortho;

		FontConstantBuffer cbData;
		DirectX::XMStoreFloat4x4(&cbData.gWorld, DirectX::XMMatrixTranspose(worldViewProj));
		cbData.uvOffset = { def.u, def.v, def.width, def.height };

		if (m_fontCBIndex >= m_fontCB.size())
		{
			m_fontCB.push_back(new ConstantBuffer<FontConstantBuffer>());
			m_fontCB.back()->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 2 + m_fontCBIndex);
		}

		m_fontCB[m_fontCBIndex]->CopyData(0, cbData);
		m_commandContext->GCommandList()->SetGraphicsRootConstantBufferView(0, m_fontCB[m_fontCBIndex]->GetAddress());

		m_commandContext->GCommandList()->DrawIndexedInstanced(m_font->GIndexCount(), 1, 0, 0, 0);

		m_fontCBIndex++;

		if (c == ' ')
		{
			cursorX += _size * 0.5f;
		}
		else
		{
			cursorX += def.advanceX * _size / 32;
		}
	}
}

void RenderEngine::DrawChar(Font* _font, char _char, Matrix _matrix)
{
	FontConstantBuffer cb;

	Matrix ortho = DirectX::XMMatrixOrthographicOffCenterLH(
		0.0f, m_swapChain->GViewport()->Width,
		m_swapChain->GViewport()->Height, 0.0f,
		0.0f, 1.0f);

	Matrix worldViewProj = ortho;
	if (m_fontCBIndex >= m_fontCB.size())
	{
		m_fontCB.push_back(new ConstantBuffer<FontConstantBuffer>());
		bool r = m_fontCB.back()->Init(m_renderDevice->GDevice(), m_desc->GcbvHeap(), 2 + m_fontCBIndex);
		Log(r, "Initializing new Font Constant Buffer");
	}
	m_fontCB[m_fontCBIndex]->CopyData(0, cb);
	auto list = m_commandContext->GCommandList();

	list->SetGraphicsRootConstantBufferView(0, m_instanceCB[m_instanceCbIndex]->GetAddress());
	_font->Bind(list);
	list->DrawIndexedInstanced(_font->GIndexCount(), 1, 0, 0, 0);
	m_instanceCbIndex++;
}
