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
	using namespace DirectX;

	// 1. Charger les données actuelles
	XMVECTOR q = XMLoadFloat4(&quaternion);
	XMVECTOR xAxis = XMLoadFloat3(&right);
	XMVECTOR yAxis = XMLoadFloat3(&up);
	XMVECTOR zAxis = XMLoadFloat3(&forward);

	// 2. Créer les rotations locales (Quaternion local = axe * angle)
	// On multiplie dans l'ordre Y -> X -> Z (Yaw, Pitch, Roll)
	if (yaw != 0.0f)   q = XMQuaternionMultiply(q, XMQuaternionRotationAxis(yAxis,yaw));
	if (pitch != 0.0f) q = XMQuaternionMultiply(q, XMQuaternionRotationAxis(xAxis,pitch));
	if (roll != 0.0f)  q = XMQuaternionMultiply(q, XMQuaternionRotationAxis(zAxis,roll));

	// 3. Normaliser pour éviter la dérive numérique
	q = XMQuaternionNormalize(q);
	XMStoreFloat4(&quaternion, q);

	// 4. Générer la matrice et extraire les nouveaux axes proprement
	XMMATRIX rot = XMMatrixRotationQuaternion(q);
	XMStoreFloat4x4(&rotmatrix, rot);

	// Extraction directe via les registres de la matrice (plus rapide)
	XMStoreFloat3(&right, rot.r[0]); // Ligne 0
	XMStoreFloat3(&up, rot.r[1]); // Ligne 1
	XMStoreFloat3(&forward, rot.r[2]); // Ligne 2
}

void Transform::SYPR(float yaw, float pitch, float roll)
{
	forward = { 0.0f, 0.0f, 1.0f };
	right = { 1.0f, 0.0f, 0.0f };
	up = { 0.0f, 1.0f, 0.0f };
	quaternion = { 0.0f, 0.0f, 0.0f, 1.0f };
	DirectX::XMStoreFloat4x4(&rotmatrix, DirectX::XMMatrixIdentity());

	AddYPR(yaw, pitch, roll);
}

Matrix4x4f Transform::BuildMatrix() const
{

	// Charger les composants en types SIMD (XMMATRIX / XMVECTOR)
	Matrix mScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
	Matrix mRot = XMLoadFloat4x4(&rotmatrix);
	Matrix mTrans = DirectX::XMMatrixTranslation(position.x, position.y, position.z);

	Matrix world = XMMatrixMultiply(mScale, mRot);

	world = XMMatrixMultiply(world, mTrans);

	Matrix4x4f finalMatrix;
	XMStoreFloat4x4(&finalMatrix, world);

	return finalMatrix;
}

void Transform::SPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Transform::SScale(float x, float y, float z)
{
	scale.x = x;
	scale.y = y;
	scale.z = z;
}