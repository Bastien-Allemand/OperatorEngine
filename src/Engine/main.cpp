#include "pch.h"
#include "Manager/GameManager.h"
#include "Manager/ComponentManager.h"
#include "Component/TransformComponent.h" 
#include "Core/Transform.h"
#include "Component/MeshComponent.h" 
#include "Render/Mesh.h"
#include "Render/Geometry.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();
	ComponentManager* componentManager = gameManager->GComponentManager();
    //spawn test entity

	uint32 id = gameManager->GNewEntityId();
	TransformComponent* transform = componentManager->AddComponent<TransformComponent>(id);
	transform->worldTransform.position = Vector3f(0, 0, 0);
	MeshComponent* mesh = componentManager->AddComponent<MeshComponent>(id);
	Geometry geometry;
	geometry.BuildBox();
	mesh->geometry = geometry;
	mesh->InitMesh();

	gameManager->Run();
    return 0;
}