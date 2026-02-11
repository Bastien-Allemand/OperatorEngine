#include "pch.h"
#include "Componant.h"

Matrix4x4f& Transform::GetWorld()
{
	return m_world;
	// TODO: insérer une instruction return ici
}

Matrix4x4f& Transform::GetInvWorld()
{
	return m_invworld;
	// TODO: insérer une instruction return ici
}

void InputComponent::BindAction(const std::string& actionName, int key)
{
    actions[actionName].keyCode = key;
    actions[actionName].isPressed = false;
    actions[actionName].isJustPressed = false;
    actions[actionName].value = 0.0f;
}

bool InputComponent::IsActionPressed(const std::string& actionName)
{
    if (actions.find(actionName) != actions.end())
    {
        return actions.at(actionName).isPressed;
    }
    return false;
}

bool InputComponent::IsActionJustPressed(const std::string& actionName)
{
    if (actions.find(actionName) != actions.end())
    {
        return actions.at(actionName).isJustPressed;
    }
    return false;
}
