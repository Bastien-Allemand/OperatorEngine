#include "pch.h"
#include "InputManager.h"

InputManager* InputManager::m_instance = nullptr;

InputManager::InputManager()
{
    DebugMsg(L"InputManager Singleton Created", DebugFlag::LOG);
}

InputManager* InputManager::GetInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new InputManager();
    }
    return m_instance;
}

void InputManager::Init(HWND hwnd)
{
    m_hwnd = hwnd;

    if (m_hwnd == nullptr)
    {
        DebugMsg(L"InputManager Init called with NULL HWND. Mouse coords will be Screen Space", DebugFlag::WARNING);
    }
    else
    {
        DebugMsg(L"InputManager Initialized successfully.", DebugFlag::LOG);
    }
}

void InputManager::Update()
{
    POINT p;
    if (GetCursorPos(&p))
    {
        if (m_hwnd != nullptr)
        {
            ScreenToClient(m_hwnd, &p);
        }

        m_mousePos = { (float32)p.x, (float32)p.y };

        m_mouseDelta.x = m_mousePos.x - m_prevMousePos.x;
        m_mouseDelta.y = m_mousePos.y - m_prevMousePos.y;

        m_prevMousePos = m_mousePos;
    }
    else
    {
        DebugMsg(L"Failed to retrieve Cursor Position", DebugFlag::WARNING);
    }

    for (auto const& [key, val] : m_keyState)
    {
        m_prevKeyState[key] = val;
    }
}

bool InputManager::IsKeyDown(int32 vKey)
{
    bool isDown = (GetAsyncKeyState(vKey) & 0x8000) != 0;

    m_keyState[vKey] = isDown;
    return isDown;
}

bool InputManager::IsKeyPressed(int32 vKey)
{
    bool isDown = IsKeyDown(vKey);
    return isDown && !m_prevKeyState[vKey];
}

bool InputManager::IsKeyReleased(int32 vKey)
{
    bool isDown = IsKeyDown(vKey);
    return !isDown && m_prevKeyState[vKey];
}

Vector2f InputManager::GetMousePosition()
{
    return m_mousePos;
}

Vector2f InputManager::GetMouseDelta()
{
    return m_mouseDelta;
}