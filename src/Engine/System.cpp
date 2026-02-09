#include "pch.h"
#include "System.h"

using namespace DirectX;

System::System()
{
    m_gameManager = GameManager::GetInstance();
}

void TransformSystem::Update(const std::vector<UINT>& entities, float deltaTime)
{
    for (UINT entityId : entities)
    {
        Transform& transform = m_gameManager->GetComponant<Transform>(entityId);
        if (transform.id != entityId) continue;

		transform.position.x *= 2; // Exemple de déplacement
		transform.position.y *= 2; // Exemple de déplacement
		transform.position.z *= 2; // Exemple de déplacement
        XMVECTOR pos = XMLoadFloat3(&transform.position);
        XMVECTOR scale = XMLoadFloat3(&transform.scale);
        XMVECTOR rot = XMLoadFloat4(&transform.quaternion);
        XMMATRIX matScale = XMMatrixScalingFromVector(scale);
        XMMATRIX matRot = XMMatrixRotationQuaternion(rot);
        XMMATRIX matTrans = XMMatrixTranslationFromVector(pos);

        XMMATRIX worldMatrix = matScale * matRot * matTrans;
        XMStoreFloat4x4(&transform.world, worldMatrix);
        XMStoreFloat3(&transform.right, XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), matRot));   
        XMStoreFloat3(&transform.up, XMVector3TransformNormal(XMVectorSet(0, 1, 0, 0), matRot));
        XMStoreFloat3(&transform.direction, XMVector3TransformNormal(XMVectorSet(0, 0, 1, 0), matRot));
    }
}