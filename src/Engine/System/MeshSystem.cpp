#include "pch.h"
#include "MeshSystem.h"

#include "Manager/GameManager.h"
#include "Render/RenderEngine.h"

#include "Component/TransformComponent.h"
#include "Component/MeshComponent.h"
#include "Manager/ComponentManager.h"
#include "ComponentPool.h"
#include "Transform.h"

void MeshSystem::Update()
{
	GameManager* gameManager = GameManager::GetInstance();
	RenderEngine* renderEngine = gameManager->GRenderEngine();
	ComponentPool<MeshComponent>* MeshPool = gameManager->GComponentManager()->GPool<MeshComponent>();
	ComponentPool<TransformComponent>* transformPool = gameManager->GComponentManager()->GPool<TransformComponent>();
	Vector<uint32>EntityWithMesh = MeshPool->GEntityIds();
	renderEngine->BeginDraw();
	for (uint32 entityId : EntityWithMesh)
	{
		TransformComponent*  transformComponent = transformPool->GComponent(entityId);
		if (transformComponent == nullptr)	continue;
		MeshComponent* meshComponent = MeshPool->GComponent(entityId);
		if (!meshComponent->visible) continue;
		if (!meshComponent->initialized) continue;
		Matrix4x4f transform = transformComponent->worldTransform.BuildMatrix();
		renderEngine->Draw(meshComponent->mesh,DirectX::XMLoadFloat4x4(&transform));
	}
	renderEngine->CloseDraw();

}
