#include "pch.h"
#include "Componant.h"

TransformComponant::TransformComponant()
{
	worldtransform.position = OPE_VEC3_ZERO; 

	worldtransform.scale = OPE_VEC3_ONE; 
	
	worldtransform.direction = OPE_VEC3_ONE; 
	worldtransform.up = OPE_VEC3_UP; 
	worldtransform.right = OPE_VEC3_DIR; 
	worldtransform.quaternion = OPE_QUAT_ZERO; 


	localTransform.position = OPE_VEC3_ZERO; 

	localTransform.scale = OPE_VEC3_ONE; 

	localTransform.direction = OPE_VEC3_ONE; 
	localTransform.up = OPE_VEC3_UP; 
	localTransform.right = OPE_VEC3_DIR; 
	localTransform.quaternion = OPE_QUAT_ZERO; 



	position = OPE_VEC3_ZERO;
	scale = OPE_VEC3_ONE;
	direction = OPE_VEC3_ONE;
	right = OPE_VEC3_DIR;
	up = OPE_VEC3_UP;
	quaternion = OPE_QUAT_ZERO;

	Matrix M;
	M.r[0] = DirectX::g_XMIdentityR0.v;
	M.r[1] = DirectX::g_XMIdentityR1.v;
	M.r[2] = DirectX::g_XMIdentityR2.v;
	M.r[3] = DirectX::g_XMIdentityR3.v;
	XMStoreFloat4x4(&rot, M);
	XMStoreFloat4x4(&localTransform.rot, M);
	XMStoreFloat4x4(&worldtransform.rot, M);

	wolrdNeedsUpdate = false;
	invWolrdNeedsUpdate = false;
}

Matrix4x4f& TransformComponant::GetWorld()
{
	return m_world;
	// TODO: insérer une instruction return ici
}

Matrix4x4f& TransformComponant::GetInvWorld()
{
	return m_invworld;
	// TODO: insérer une instruction return ici
}

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
