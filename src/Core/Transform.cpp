#include "pch.h"
#include "Transform.h"

void Transform::MoveFoward(float distance)
{
	position.x += forward.x * distance;
	position.y += forward.y * distance;
	position.z += forward.z * distance;
}

void Transform::MoveRight(float distance)
{
	position.x += right.x * distance;
	position.y += right.y * distance;
	position.z += right.z * distance;
}

void Transform::MoveUp(float distance)
{
	position.x += up.x * distance;
	position.y += up.y * distance;
	position.z += up.z * distance;
}

void Transform::AddYPR(float yaw, float pitch, float roll)
{
	XVector yAxis = DirectX::XMLoadFloat3(&up);
	XVector xAxis = DirectX::XMLoadFloat3(&right);
	XVector zAxis = DirectX::XMLoadFloat3(&forward);

	XVector quat = DirectX::XMLoadFloat4(&quaternion);

	quat = DirectX::XMQuaternionMultiply(quat, DirectX::XMQuaternionRotationAxis(yAxis, DirectX::XMConvertToRadians(yaw)));
	quat = DirectX::XMQuaternionMultiply(quat, DirectX::XMQuaternionRotationAxis(xAxis, DirectX::XMConvertToRadians(pitch)));
	quat = DirectX::XMQuaternionMultiply(quat, DirectX::XMQuaternionRotationAxis(zAxis, DirectX::XMConvertToRadians(roll)));

	quat = DirectX::XMQuaternionNormalize(quat);
	DirectX::XMStoreFloat4(&quaternion, quat);

	DirectX::XMStoreFloat4x4(&rotmatrix, DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&quaternion)));
	right.x = rotmatrix._11;
	right.y = rotmatrix._12;
	right.z = rotmatrix._13;
	up.x = rotmatrix._21;
	up.y = rotmatrix._22;
	up.z = rotmatrix._23;
	forward.x = rotmatrix._31;
	forward.y = rotmatrix._32;
	forward.z = rotmatrix._33;
}

void Transform::SetYPR(float yaw, float pitch, float roll)
{
	forward = { 0.0f, 0.0f, 1.0f };
	right = { 1.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };
	quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMStoreFloat4x4(&rotmatrix, DirectX::XMMatrixIdentity());

	AddYPR(yaw, pitch, roll);
}

DirectX::XMFLOAT4X4 Transform::BuildMatrix() const
{
	Matrix result;

	Matrix transMatrix = DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	Matrix scaleMatrix = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);

	result = DirectX::XMMatrixMultiply(scaleMatrix, transMatrix);
	result = DirectX::XMMatrixMultiply(DirectX::XMLoadFloat4x4(&rotmatrix), result);

	Matrix4x4f finalMatrix;
	DirectX::XMStoreFloat4x4(&finalMatrix, result);

	return finalMatrix;
}

void Transform::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SetScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}