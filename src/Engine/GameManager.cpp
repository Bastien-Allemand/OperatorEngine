#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
	return (m_instance == nullptr) ? new GameManager() : m_instance;
}