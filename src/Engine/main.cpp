#include "pch.h"
#include "main.h"
#include "GameManager.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	GameManager* gameManager = GameManager::GetInstance();
	gameManager->AddComponant<Transform>(0);
	gameManager->GetComponant<Transform>(0);
    return 0;
}