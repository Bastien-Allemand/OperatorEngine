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
    InputComponent& inp1 = gameManager->AddComponant<InputComponent>(entity1.id.first);
    TransformSystem& transSys = gameManager->AddSystem<TransformSystem>(entity1.id.first);
    InputSystem& inputSys = gameManager->AddSystem<InputSystem>(entity1.id.first);
    float dt = 0.016f;
    inputSys.Update({ entity1.id.first }, dt);
    transSys.Update({ entity1.id.first }, dt);
    Transform& res = gameManager->GetComponant<Transform>(entity1.id.first);
    std::cout << "Pos Z apres Input: " << res.position.z << std::endl;


    // partie Loïc
	Entity* entity1 = gameManager->AddEntity(0);
    Entity* entity2 = gameManager->AddEntity(1);
    Entity* entity3 = gameManager->AddEntity(2);

    TransformComponant&  a = gameManager->AddComponant<TransformComponant>(entity1);
    TransformComponant& b = gameManager->AddComponant<TransformComponant>(entity2);
    TransformComponant& c = gameManager->AddComponant<TransformComponant>(entity3);

    entity1->AddChild(entity2);

    TransformSystem& transformSystem = gameManager->AddSystem<TransformSystem>(entity1);
    TransformSystem& transformSystem2 = gameManager->AddSystem<TransformSystem>(entity2);
    TransformSystem& transformSystem3 = gameManager->AddSystem<TransformSystem>(entity3);

    gameManager->Update();
    TransformComponant& retrievedTransform1 = gameManager->GetComponant<TransformComponant>(entity1);
    TransformComponant& retrievedTransform2 = gameManager->GetComponant<TransformComponant>(entity2);
    TransformComponant& retrievedTransform3 = gameManager->GetComponant<TransformComponant>(entity3);


        // Test pour voir si �a marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransformfeu1.position.y << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
        std::cout << "Position X : " << retrievedTransform3.position.x << std::endl;


    return 0;
}