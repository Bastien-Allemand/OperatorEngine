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
        

		transform.position.x += 0.1f * deltaTime; // Exemple de d�placement
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

void InputSystem::Update(const std::vector<UINT>& entities, float deltaTime)
{
    InputManager* im = InputManager::GetInstance();
    im->Update();

    for (UINT id : entities)
    {
        try
        {
            InputComponent& inputComp = m_gameManager->GetComponant<InputComponent>(id);
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