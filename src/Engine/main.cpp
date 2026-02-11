#include "pch.h"
#include "main.h"
#include "GameManager.h"
#include "Entity.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();

    Entity entity1;
    entity1.id = std::make_pair(0, 1);
    gameManager->AddEntity(entity1.id.first, true, true);

    Transform& t1 = gameManager->AddComponant<Transform>(entity1.id.first);
    t1.position = Vector3f(0.0f, 0.0f, 0.0f);

    Input& inp1 = gameManager->AddComponant<Input>(entity1.id.first);

    TransformSystem& transSys = gameManager->AddSystem<TransformSystem>(entity1.id.first);
    InputSystem& inputSys = gameManager->AddSystem<InputSystem>(entity1.id.first);

    float dt = 0.016f;

    inputSys.Update({ entity1.id.first }, dt);
    transSys.Update({ entity1.id.first }, dt);

    Transform& res = gameManager->GetComponant<Transform>(entity1.id.first);

    return 0;
}