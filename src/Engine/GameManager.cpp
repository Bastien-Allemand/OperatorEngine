#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
	//return (m_instance == nullptr) ? new GameManager() : m_instance;
    if (m_instance == nullptr) {
        m_instance = new GameManager();
    }
    return m_instance;
}