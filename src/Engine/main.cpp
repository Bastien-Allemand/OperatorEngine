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

	a.position = Vector3f(1.0f, 2.0f, 3.0f);
    InputComponent& inputs = gameManager->AddComponant<InputComponent>(entity1.id.first);

    TransformSystem& transSys = gameManager->AddSystem<TransformSystem>(entity1.id.first);
    InputSystem& inputSys = gameManager->AddSystem<InputSystem>(entity1.id.first);

    float dt = 0.016f;

    inputSys.Update({ entity1.id.first }, dt);
    transSys.Update({ entity1.id.first }, dt);
    Transform& retrievedTransform1 = gameManager->GetComponant<Transform>(entity1.id.first);


    //feu& retrievedTransformfeu1 = gameManager->GetComponant<feu>(entity1.id.first);

    Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(entity2.id.first);
    //feu& retrievedTransformfeu2 = gameManager->GetComponant<feu>(entity2.id.first);

    TransformSystem retrievedSystem1;

    retrievedSystem1.Update({ entity1.id.first, entity2.id.first }, 0.016f);

        // Test pour voir si �a marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransformfeu1.position.y << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;

    Transform& res = gameManager->GetComponant<Transform>(entity1.id.first);

    return 0;
}