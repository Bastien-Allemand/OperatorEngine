#include "pch.h"
#include "MeshComponent.h"
#include "Manager/GameManager.h"
#include "Render/RenderEngine.h"

void MeshComponent::InitMesh()
{
	mesh = new Mesh(geometry);	
	GameManager* gameManager = GameManager::GetInstance();
	RenderEngine* renderEngine = gameManager->GRenderEngine();

	renderEngine->InitMesh(mesh);
	initialized = true;
}
