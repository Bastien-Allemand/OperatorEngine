#pragma once
#include "Component.h"
struct InputAction
{
    int keyCode;
    bool isPressed = false;
    bool isJustPressed = false;
    float value = 0.0f;
};

struct InputComponent : public Component
{
    InputComponent()
    {
        type = ComponentType::Input;
    }
    std::unordered_map<String, InputAction> actions;
    void BindAction(const std::string& _actionName, int key);
    bool IsActionPressed(const std::string& _actionName);
    bool IsActionJustPressed(const std::string& _actionName);
};