#include "pch.h"
#include "System.h"
#include "Manager/InputManager.h"
#include "Component.h"
#include "Manager/GameManager.h"
#include "Entity.h"

void TransformSystem::Update()
{
    //for (Entity* entity : m_entities)
    //{
    //    TransformComponant& transform = m_gameManager->GetComponant<TransformComponant>(entity);


    //    transform.localTransform.position.x += 0.1f * deltaTime; // Exemple de d�placement
    //    XVector pos = XMLoadFloat3(&transform.worldTransform.position);
    //    XVector rot = XMLoadFloat4(&transform.worldTransform.quaternion);
    //    XVector scale = XMLoadFloat3(&transform.worldTransform.scale);
    //    Matrix matScale = DirectX::XMMatrixScalingFromVector(scale);
    //    Matrix matRot = DirectX::XMMatrixRotationQuaternion(rot);
    //    Matrix matTrans = DirectX::XMMatrixTranslationFromVector(pos);

    //    DirectX::XMMATRIX worldMatrix = matScale * matRot * matTrans;
    //    DirectX::XMStoreFloat4x4(&transform.m_world, worldMatrix);

    //    DirectX::XMStoreFloat3(&transform.worldTransform.right, XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), matRot));
    //    DirectX::XMStoreFloat3(&transform.worldTransform.up, XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), matRot));
    //    DirectX::XMStoreFloat3(&transform.worldTransform.forward, XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), matRot));


    //    //==========================================================================
    //    DirectX::XMVECTOR s = XMLoadFloat3(&transform.worldTransform.scale);
    //    DirectX::XMVECTOR p = XMLoadFloat3(&transform.worldTransform.position);

    //    DirectX::XMVECTOR sx = DirectX::XMVectorSplatX(s);
    //    DirectX::XMVECTOR sy = DirectX::XMVectorSplatY(s);
    //    DirectX::XMVECTOR sz = DirectX::XMVectorSplatZ(s);
    //    DirectX::XMMATRIX w = DirectX::XMLoadFloat4x4(&transform.worldTransform.rotmatrix);
    //    w.r[0] = DirectX::XMVectorMultiply(w.r[0], sx);
    //    w.r[1] = DirectX::XMVectorMultiply(w.r[1], sy);
    //    w.r[2] = DirectX::XMVectorMultiply(w.r[2], sz);
    //    w.r[3] = DirectX::XMVectorSetW(p, 1.0f);

    //    XMStoreFloat4x4(&transform.m_world, w);
    //    transform.wolrdNeedsUpdate = true;
    //    transform.invWolrdNeedsUpdate = false;
    //}
}

void TransformSystem::Move()
{
    //for (Entity* entity : m_entitiesss)
    //{
    //    Transform& transform = m_gameManager->GetComponant<Transform>(entity);
    //    //if (transform.m_children.size() > 0) {
    //    //    for (Transform* child : transform.m_children) {
    //    //        child->position.x += 10.1f; // Exemple de d�placement pour les enfants
    //    //    }
    //    //}

    //    if (entity->m_children.size() > 0) {
    //        for (Entity* child : entity->m_children)
    //        {
    //            Transform& transformChild = m_gameManager->GetComponant<Transform>(child); transformChild.position.x += 10.1f;
    //        }
    //    }
    //    transform.position.x += 10.1f; // Exemple de d�placement

    //}
}

void InputSystem::Update()
{
    //InputManager* im = InputManager::GetInstance();
    //im->Update();

    //for (Entity* entity : m_entitiesss)
    //{
    //    try
    //    {
    //        InputComponent& inputComp = m_gameManager->GetComponant<InputComponent>(entity);
    //        if (inputComp.id != id) continue;
    //        for (auto& [name, action] : inputComp.actions)
    //        {
    //            bool currentlyDown = im->IsKeyDown(action.keyCode);
    //            bool justPressed = im->IsKeyPressed(action.keyCode);

    //            action.isPressed = currentlyDown;
    //            action.isJustPressed = justPressed;
    //            action.value = currentlyDown ? 1.0f : 0.0f;
    //        }
    //    }
    //    catch (const std::exception&) { continue; }
    //}
}

