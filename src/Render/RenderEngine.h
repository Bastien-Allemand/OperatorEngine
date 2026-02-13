#pragma once

class Mesh;
class Font;
class Fence;
class Factory;
class SwapChain;
class Descriptors;
class RenderDevice;
class RenderTarget;
class CommandContext;
class PipelineStateObject;
class PipelineStateObject2D;

template<typename T>
class ConstantBuffer;

struct ConstantBufferPass {
	Matrix4x4f gWorld;
	Matrix4x4f gViewProj;
};
struct alignas(16) DirectionalLight {
	Vector4f Ambient;
	Vector4f Diffuse;
	Vector4f Specular;
	Vector3f Direction;
	float Pad;
};
struct LightData
{
	DirectionalLight DirLight;
	Vector3f EyePosW;
};
struct FontConstantBuffer
{
	Matrix4x4f gWorld;
	Vector4f uvOffset;
};


class RenderEngine
{
public:
	RenderEngine() = default;
	~RenderEngine();

	bool Init(int _width, int _height, HWND _handle);
	void Update(float dt);
	void InitMesh(Mesh* _mesh);
	void InitFont(Font* _font);
	void BeginDraw();
	void CloseDraw();
	void Draw(Mesh* _mesh, Matrix _matrix);
	void DrawChar(Font* _font, char _char, Matrix _matrix);
	bool Resize(int _width, int _height);
	bool m4xMsaaState = 0;
	uint32 m4xMsaaQuality = 0;

private:
	Factory* m_factory = nullptr;
	RenderDevice* m_renderDevice = nullptr;
	CommandContext* m_commandContext = nullptr;
	PipelineStateObject* m_pipelineStateObject = nullptr;
	PipelineStateObject2D* m_pso2D = nullptr;
	SwapChain* m_swapChain = nullptr;
	ID3D12CommandQueue* m_queue = nullptr;
	Descriptors* m_desc;
	RenderTarget* m_renderTarget;
	Fence* m_fence = nullptr;

	Vector<Mesh*> m_meshes;
	Font* m_font = nullptr;  // Système de font

	// Heap SRV pour la texture de font
	ID3D12DescriptorHeap* m_fontSrvHeap = nullptr;

	ConstantBuffer<ConstantBufferPass>* m_sceneCB = nullptr;
	ConstantBuffer<LightData>* m_lightCB = nullptr;

	Vector<ConstantBuffer <FontConstantBuffer>*> m_fontCB;
	int m_fontCBIndex = 0;

	Vector<ConstantBuffer<ConstantBufferPass>*> m_instanceCB;
	int m_instanceCbIndex = 0;

	Mesh* m_quadMesh = nullptr;

	Matrix4x4f m_world;
	Matrix4x4f m_view;
	Matrix4x4f m_proj;
	LightData m_lightData;
	float mTheta = 1.5f * DirectX::XM_PI;
	float mPhi = DirectX::XM_PIDIV4;
	float mRadius = 5.0f;

	bool FlushCommandQueue();
	void HardInit();
	bool InitQueue();
	bool InitFont();
	void DrawString(String _text, float32 _x, float32 _y, float32 _size);	
};

