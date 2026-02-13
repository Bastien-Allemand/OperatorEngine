#include "pch.h"
#include "System.h"
#include "InputManager.h"
#include "Componant.h"

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

Entity* System::GetEntity(UINT id)
{
    for (Entity* entity : m_entitiesss)
    {
        if (entity->id.first == id) return entity;
    }
    return nullptr;
}

void TransformSystem::Update(float deltaTime)
{
    for (Entity* entity : m_entitiesss)
    {
        TransformComponant& transform = m_gameManager->GetComponant<TransformComponant>(entity);


        transform.position.x += 0.1f * deltaTime; // Exemple de d�placement
        XVector pos = XMLoadFloat3(&transform.worldtransform.position);
        XVector rot = XMLoadFloat4(&transform.worldtransform.quaternion);
        XVector scale = XMLoadFloat3(&transform.worldtransform.scale);
        Matrix matScale = DirectX::XMMatrixScalingFromVector(scale);
        Matrix matRot = DirectX::XMMatrixRotationQuaternion(rot);
        Matrix matTrans = DirectX::XMMatrixTranslationFromVector(pos);

        DirectX::XMMATRIX worldMatrix = matScale * matRot * matTrans;
        DirectX::XMStoreFloat4x4(&transform.GetWorld(), worldMatrix);
        DirectX::XMStoreFloat3(&transform.worldtransform.right, XMVector3TransformNormal(DirectX::XMVectorSet(1, 0, 0, 0), matRot));
        DirectX::XMStoreFloat3(&transform.worldtransform.up, XMVector3TransformNormal(DirectX::XMVectorSet(0, 1, 0, 0), matRot));
        DirectX::XMStoreFloat3(&transform.worldtransform.direction, XMVector3TransformNormal(DirectX::XMVectorSet(0, 0, 1, 0), matRot));


        //==========================================================================
        DirectX::XMVECTOR s = XMLoadFloat3(&transform.worldtransform.scale);
        DirectX::XMVECTOR p = XMLoadFloat3(&transform.worldtransform.position);

        DirectX::XMVECTOR sx = DirectX::XMVectorSplatX(s);
        DirectX::XMVECTOR sy = DirectX::XMVectorSplatY(s);
        DirectX::XMVECTOR sz = DirectX::XMVectorSplatZ(s);
        DirectX::XMMATRIX w = XMLoadFloat4x4(&transform.worldtransform.rot);
        w.r[0] = DirectX::XMVectorMultiply(w.r[0], sx);
        w.r[1] = DirectX::XMVectorMultiply(w.r[1], sy);
        w.r[2] = DirectX::XMVectorMultiply(w.r[2], sz);
        w.r[3] = DirectX::XMVectorSetW(p, 1.0f);

        XMStoreFloat4x4(&transform.GetWorld(), w);
        transform.wolrdNeedsUpdate = true;
        transform.invWolrdNeedsUpdate = false;
    }
}

void TransformSystem::Move()
{
    for (Entity* entity : m_entitiesss)
    {
        Transform& transform = m_gameManager->GetComponant<Transform>(entity);
        //if (transform.m_children.size() > 0) {
        //    for (Transform* child : transform.m_children) {
        //        child->position.x += 10.1f; // Exemple de d�placement pour les enfants
        //    }
        //}

        if (entity->m_children.size() > 0) {
            for (Entity* child : entity->m_children)
            {
                Transform& transformChild = m_gameManager->GetComponant<Transform>(child); transformChild.position.x += 10.1f;
            }
        }
        transform.position.x += 10.1f; // Exemple de d�placement

    }
}

void InputSystem::Update(float deltaTime)
{
    InputManager* im = InputManager::GetInstance();
    im->Update();

    for (Entity* entity : m_entitiesss)
    {
        try
        {
            InputComponent& inputComp = m_gameManager->GetComponant<InputComponent>(entity);
            if (inputComp.id != id) continue;
            for (auto& [name, action] : inputComp.actions)
            {
                bool currentlyDown = im->IsKeyDown(action.keyCode);
                bool justPressed = im->IsKeyPressed(action.keyCode);

                action.isPressed = currentlyDown;
                action.isJustPressed = justPressed;
                action.value = currentlyDown ? 1.0f : 0.0f;
            }
        }
        catch (const std::exception&) { continue; }
    }
}

void CollisionSystem::Update(float deltaTime)
{
   std::vector<BoxCollider*> colliders;
   for (Entity* e : m_entitiesss) colliders.push_back(&m_gameManager->GetComponant<BoxCollider>(e));
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


    /*for (Entity* entity1 : m_entitiesss)
    {
		BoxCollider& collider1 = m_gameManager->GetComponant<BoxCollider>(entity1);

        if (collider1.IsColliding) continue;
        for (Entity* entity2 : m_entitiesss)
        {
            BoxCollider& collider2 = m_gameManager->GetComponant<BoxCollider>(entity2);
			if (entity1 == entity2 || collider2.IsColliding) continue;

            if (collider1.min.x <= collider2.max.x && collider1.max.x >= collider2.min.x &&
                collider1.min.y <= collider2.max.y && collider1.max.y >= collider2.min.y &&
				collider1.min.z <= collider2.max.z && collider1.max.z >= collider2.min.z)
            {
                collider1.IsColliding = true;
                collider2.IsColliding = true;
			}
        }
    }*/
}

void InputSystem::Update(float deltaTime)
{
}
