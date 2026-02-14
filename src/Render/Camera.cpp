#include "pch.h"
#include "Camera.h"
#include "Core/Transform.h"

Camera::Camera()
{
	transform = new Transform();
}

Matrix Camera::BuildViewMatrix() const
{
	XVector pos = DirectX::XMVectorSet(transform->position.x, transform->position.y, transform->position.z, 1);
	XVector foward = { transform->forward.x,transform->forward.y,transform->forward.z,0.0f };
	XVector target = DirectX::XMVectorAdd(pos, foward);
	XVector up = DirectX::XMVectorSet(transform->up.x, transform->up.y, transform->up.z, 0.0f);
	return DirectX::XMMatrixLookAtLH(pos, target, up);
}
