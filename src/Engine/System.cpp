#include "pch.h"
#include "System.h"

System::System()
{
    m_gameManager = GameManager::GetInstance();
}

void System::AddEntity(UINT entity) {
    if (entity >= m_entities.size()) {
        m_entities.resize(entity + 1);
		m_entities[entity] = entity;
    }
}

void TransformSystem::Update(const std::vector<UINT>& entities, float deltaTime)
{
    for (UINT i = 0; i < entities.size(); ++i) {
        Transform& transform = m_gameManager->GetComponant<Transform>(i);
        if (transform.id != i) continue;
        

		transform.position.x += 0.1f * deltaTime; // Exemple de déplacement
        Vector pos = XMLoadFloat3(&transform.position);
        Vector scale = XMLoadFloat3(&transform.scale);
        Vector rot = XMLoadFloat4(&transform.quaternion);
        Matrix matScale = DirectX::XMMatrixScalingFromVector(scale);
        Matrix matRot = DirectX::XMMatrixRotationQuaternion(rot);
        Matrix matTrans = DirectX::XMMatrixTranslationFromVector(pos);

        DirectX::XMMATRIX worldMatrix = matScale * matRot * matTrans;
        DirectX::XMStoreFloat4x4(&transform.GetWorld(), worldMatrix);
        DirectX::XMStoreFloat3(&transform.right, XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), matRot));
        DirectX::XMStoreFloat3(&transform.up, XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), matRot));
        DirectX::XMStoreFloat3(&transform.direction, XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), matRot));


        //==========================================================================
        DirectX::XMVECTOR s = XMLoadFloat3(&transform.scale);
        DirectX::XMVECTOR p = XMLoadFloat3(&transform.position);

        DirectX::XMVECTOR sx = DirectX::XMVectorSplatX(s);
        DirectX::XMVECTOR sy = DirectX::XMVectorSplatY(s);
        DirectX::XMVECTOR sz = DirectX::XMVectorSplatZ(s);
        DirectX::XMMATRIX w = XMLoadFloat4x4(&transform.rot);
        w.r[0] = DirectX::XMVectorMultiply(w.r[0], sx);
        w.r[1] = DirectX::XMVectorMultiply(w.r[1], sy);
        w.r[2] = DirectX::XMVectorMultiply(w.r[2], sz);
        w.r[3] = DirectX::XMVectorSetW(p, 1.0f);
          
        XMStoreFloat4x4(&transform.GetWorld(), w);
        transform.wolrdNeedsUpdate = true;
        transform.invWolrdNeedsUpdate = false;



    }
}