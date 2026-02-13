#include "pch.h"
#include "Manager/GameManager.h"
#include "Entity.h"
#include "Core/Transform.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();
	gameManager->Run();
    return 0;
}