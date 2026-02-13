#pragma once
#include "../Core/Define.h"
#include <unordered_map>

class InputManager
{
public:
    static InputManager* GetInstance();

    void Init(HWND hwnd);

    void Update();

    // Clavier
    bool IsKeyDown(int32 vKey);      
    bool IsKeyPressed(int32 vKey);   
    bool IsKeyReleased(int32 vKey);  

    // Souris
    Vector2f GetMousePosition();
    Vector2f GetMouseDelta();

private:
    InputManager();
    ~InputManager() = default;

    static InputManager* m_instance;

    HWND m_hwnd = nullptr;

    // Gestion des états
    std::unordered_map<int32, bool> m_keyState;
    std::unordered_map<int32, bool> m_prevKeyState;

    Vector2f m_mousePos = { 0.0f, 0.0f };
    Vector2f m_prevMousePos = { 0.0f, 0.0f };
    Vector2f m_mouseDelta = { 0.0f, 0.0f };
};