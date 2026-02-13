#include "pch.h"
#include "Component.h"


void InputComponent::BindAction(const std::string& _actionName, int key)
{
    actions[_actionName].keyCode = key;
    actions[_actionName].isPressed = false;
    actions[_actionName].isJustPressed = false;
    actions[_actionName].value = 0.0f;
}

bool InputComponent::IsActionPressed(const std::string& _actionName)
{
    if (actions.find(_actionName) != actions.end())
    {
        return actions.at(_actionName).isPressed;
    }
    return false;
}

bool InputComponent::IsActionJustPressed(const std::string& _actionName)
{
    if (actions.find(_actionName) != actions.end())
    {
        return actions.at(_actionName).isJustPressed;
    }
    return false;
}
