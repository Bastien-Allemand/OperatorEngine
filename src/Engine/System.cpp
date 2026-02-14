#include "pch.h"
#include "System.h"
#include "Manager/InputManager.h"
#include "ComponentPool.h"
#include "Manager/GameManager.h"
#include "Entity.h"

//void TransformSystem::Update()
//{
//    //for (Entity* entity : m_entities)
//    //{
//    //    TransformComponant& transform = m_gameManager->GetComponant<TransformComponant>(entity);
//
//
//    //    transform.localTransform.position.x += 0.1f * deltaTime; // Exemple de d�placement
//    //    XVector pos = XMLoadFloat3(&transform.worldTransform.position);
//    //    XVector rot = XMLoadFloat4(&transform.worldTransform.quaternion);
//    //    XVector scale = XMLoadFloat3(&transform.worldTransform.scale);
//    //    Matrix matScale = DirectX::XMMatrixScalingFromVector(scale);
//    //    Matrix matRot = DirectX::XMMatrixRotationQuaternion(rot);
//    //    Matrix matTrans = DirectX::XMMatrixTranslationFromVector(pos);
//
//    //    DirectX::XMMATRIX worldMatrix = matScale * matRot * matTrans;
//    //    DirectX::XMStoreFloat4x4(&transform.m_world, worldMatrix);
//
//    //    DirectX::XMStoreFloat3(&transform.worldTransform.right, XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), matRot));
//    //    DirectX::XMStoreFloat3(&transform.worldTransform.up, XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), matRot));
//    //    DirectX::XMStoreFloat3(&transform.worldTransform.forward, XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), matRot));
//
//
//    //    //==========================================================================
//    //    DirectX::XMVECTOR s = XMLoadFloat3(&transform.worldTransform.scale);
//    //    DirectX::XMVECTOR p = XMLoadFloat3(&transform.worldTransform.position);
//
//    //    DirectX::XMVECTOR sx = DirectX::XMVectorSplatX(s);
//    //    DirectX::XMVECTOR sy = DirectX::XMVectorSplatY(s);
//    //    DirectX::XMVECTOR sz = DirectX::XMVectorSplatZ(s);
//    //    DirectX::XMMATRIX w = DirectX::XMLoadFloat4x4(&transform.worldTransform.rotmatrix);
//    //    w.r[0] = DirectX::XMVectorMultiply(w.r[0], sx);
//    //    w.r[1] = DirectX::XMVectorMultiply(w.r[1], sy);
//    //    w.r[2] = DirectX::XMVectorMultiply(w.r[2], sz);
//    //    w.r[3] = DirectX::XMVectorSetW(p, 1.0f);
//
//    //    XMStoreFloat4x4(&transform.m_world, w);
//    //    transform.wolrdNeedsUpdate = true;
//    //    transform.invWolrdNeedsUpdate = false;
//    //}
//}
//
//void TransformSystem::Move()
//{
//    //for (Entity* entity : m_entitiesss)
//    //{
//    //    Transform& transform = m_gameManager->GetComponant<Transform>(entity);
//    //    //if (transform.m_children.size() > 0) {
//    //    //    for (Transform* child : transform.m_children) {
//    //    //        child->position.x += 10.1f; // Exemple de d�placement pour les enfants
//    //    //    }
//    //    //}
//
//    //    if (entity->m_children.size() > 0) {
//    //        for (Entity* child : entity->m_children)
//    //        {
//    //            Transform& transformChild = m_gameManager->GetComponant<Transform>(child); transformChild.position.x += 10.1f;
//    //        }
//    //    }
//    //    transform.position.x += 10.1f; // Exemple de d�placement
//
//    //}
//}

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

void CollisionSystem::Update(float deltaTime)
{
   std::vector<BoxCollider*> colliders;
   for (Entity* e : m_entitiesss)
   {
	   BoxCollider* collider = &m_gameManager->GetComponant<BoxCollider>(e);
       TransformComponant& transform = m_gameManager->GetComponant<TransformComponant>(e);

       //Vector3f halfSize;

       //halfSize.x = (collider->size.x * transform.worldTransform.scale.x) * collider->size.x;
       //halfSize.y = (collider->size.y * transform.worldTransform.scale.y) * collider->size.y;
       //halfSize.z = (collider->size.z * transform.worldTransform.scale.z) * collider->size.z;

       collider->center = transform.worldTransform.position;

	   collider->min.x = collider->center.x - collider->size.x;
	   collider->min.y = collider->center.y - collider->size.y;
	   collider->min.z = collider->center.z - collider->size.z;

	   collider->max.x = collider->center.x + collider->size.x;
	   collider->max.y = collider->center.y + collider->size.y;
	   collider->max.z = collider->center.z + collider->size.z;

       collider->IsColliding = false;
       colliders.push_back(collider);


   }
    for (int i = 0; i < colliders.size(); ++i)
    {
        BoxCollider* collider1 = colliders[i];
        if (collider1->IsColliding) continue;
        for (int j = i + 1; j < colliders.size(); ++j)
        {
            BoxCollider* collider2 = colliders[j];
            if (collider2->IsColliding || collider1 == collider2) continue;
            if (collider1->min.x <= collider2->max.x && collider1->max.x >= collider2-> min.x &&
                collider1->min.y <= collider2->max.y && collider1->max.y >= collider2-> min.y &&
                collider1->min.z <= collider2->max.z && collider1->max.z >= collider2-> min.z)
            {
                collider1->IsColliding = true;
                collider2->IsColliding = true;
            }
        }
	}
}
