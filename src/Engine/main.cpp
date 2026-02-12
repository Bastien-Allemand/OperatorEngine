#include "pch.h"
#include "main.h"
#include "GameManager.h"
#include "Entity.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();
    Entity* entity1 = gameManager->AddEntity(1);
    TransformComponant& t1 = gameManager->AddComponant<TransformComponant>(entity1);
    t1.position = Vector3f(0.0f, 0.0f, 0.0f);
    InputComponent& inp1 = gameManager->AddComponant<InputComponent>(entity1);
    TransformSystem& transSys = gameManager->AddSystem<TransformSystem>(entity1);
    InputSystem& inputSys = gameManager->AddSystem<InputSystem>(entity1);
    Transform& res = gameManager->GetComponant<Transform>(entity1);
    std::cout << "Pos Z apres Input: " << res.position.z << std::endl;


    // partie Loïc
	Entity* entity4 = gameManager->AddEntity(2);
    Entity* entity5 = gameManager->AddEntity(3);
    Entity* entity6 = gameManager->AddEntity(4);

    TransformComponant&  a = gameManager->AddComponant<TransformComponant>(entity4);
    TransformComponant& b = gameManager->AddComponant<TransformComponant>(entity5);
    TransformComponant& c = gameManager->AddComponant<TransformComponant>(entity6);

    entity6->AddChild(entity4);

    TransformSystem& transformSystem = gameManager->AddSystem<TransformSystem>(entity4);
    TransformSystem& transformSystem2 = gameManager->AddSystem<TransformSystem>(entity5);
    TransformSystem& transformSystem3 = gameManager->AddSystem<TransformSystem>(entity6);

    gameManager->Update();
    TransformComponant& retrievedTransform1 = gameManager->GetComponant<TransformComponant>(entity4);
    TransformComponant& retrievedTransform2 = gameManager->GetComponant<TransformComponant>(entity5);
    TransformComponant& retrievedTransform3 = gameManager->GetComponant<TransformComponant>(entity6);


        // Test pour voir si �a marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransformfeu1.position.y << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
        std::cout << "Position X : " << retrievedTransform3.position.x << std::endl;


    return 0;
}