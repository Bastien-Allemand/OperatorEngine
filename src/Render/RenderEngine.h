#pragma once

class Mesh;
class Fence;
class Camera;
class Factory;
class SwapChain;
class Descriptors;
class RenderDevice;
class RenderTarget;
class CommandContext;
class PipelineStateObject;

template<typename T>
class ConstantBuffer;

struct ConstantBufferPass {
	Matrix4x4f gWorld;
	Matrix4x4f gViewProj;
};
struct alignas(16) DirectionalLight {
	Vector4f Ambient;   // 16 octets
	Vector4f Diffuse;   // 16 octets
	Vector4f Specular;  // 16 octets
	Vector3f Direction; // 12 octets
	float Pad;
};
struct LightData
{
	DirectionalLight DirLight;
	Vector3f EyePosW;
};

class RenderEngine
{
public:
	RenderEngine() = default;
	~RenderEngine();

	bool Init(int _width, int _height, HWND _handle);
	void Update(float dt);
	void InitMesh(Mesh* _mesh);
	void BeginDraw();
	void CloseDraw();
	void Draw(Mesh* _mesh, Matrix _matrix);
	bool Resize(int _width, int _height);
	Camera* GCamera() const { return m_camera; }
	bool m4xMsaaState = 0;
	uint32 m4xMsaaQuality = 0;

private:
	Factory* m_factory = nullptr;

	RenderDevice* m_renderDevice = nullptr;

	CommandContext* m_commandContext = nullptr;

	PipelineStateObject* m_pipelineStateObject = nullptr;

	SwapChain* m_swapChain = nullptr;

	ID3D12CommandQueue* m_queue = nullptr;

	Descriptors* m_desc;

	RenderTarget* m_renderTarget;

	Fence* m_fence = nullptr;

	Vector<Mesh*> m_meshes;

	ConstantBuffer<ConstantBufferPass>* m_sceneCB = nullptr;
	ConstantBuffer<LightData>* m_lightCB = nullptr;

	Vector<ConstantBuffer<ConstantBufferPass>*> m_instanceCB;
	int m_instanceCbIndex = 0;

	Mesh* m_quadMesh = nullptr;

	Matrix4x4f m_world;
	Matrix4x4f m_viewProj;
	Matrix4x4f m_proj;
	Camera* m_camera = nullptr;
	LightData m_lightData;

	bool FlushCommandQueue();

	void HardInit();
	bool InitQueue();
};

