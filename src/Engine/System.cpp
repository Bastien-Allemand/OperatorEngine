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

void TransformSystem::Update( float deltaTime)
{
    for (Entity* entity : m_entitiesss)
    {
        Transform& transform = m_gameManager->GetComponant<Transform>(entity);
        

		transform.position.x += 0.1f * deltaTime; // Exemple de déplacement
        XVector pos = XMLoadFloat3(&transform.position);
        XVector rot = XMLoadFloat4(&transform.quaternion);
        XVector scale = XMLoadFloat3(&transform.scale);
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

void TransformSystem::AddChildren(Transform& child)
{
    Transform& transform = m_gameManager->GetComponant<Transform>(id);
    transform.m_children.push_back(&child);
}

void TransformSystem::AddParent(Transform& parent)
{
    Transform& transform = m_gameManager->GetComponant<Transform>(id);
	parent.m_children.push_back(&transform);
}

void TransformSystem::Move()
{
    for (Entity* entity : m_entitiesss)
    {
        Transform& transform = m_gameManager->GetComponant<Transform>(entity);

        
    }
 //   Transform& transform = m_gameManager->GetComponant<Transform>(id);
 //   if (transform.m_children.size() > 0) {
 //       for (Transform* child : transform.m_children) {
 //           child->position.x += 10.1f; // Exemple de déplacement pour les enfants
 //       }
	//}
	//transform.position.x += 10.1f; // Exemple de déplacement
}
