#include "pch.h"
#include "main.h"
#include "GameManager.h"
#include "Entity.h"
// Assurez-vous que Transform est défini ou inclus

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();

	Entity* entity1 = gameManager->AddEntity(0, true, true);
    Entity* entity2 = gameManager->AddEntity(1, true, true);
    Entity* entity3 = gameManager->AddEntity(2, true, true);

    Transform&  a = gameManager->AddComponant<Transform>(entity1);
    Transform& b = gameManager->AddComponant<Transform>(entity2);
    Transform& c = gameManager->AddComponant<Transform>(entity3);


    TransformSystem& transformSystem = gameManager->AddSystem<TransformSystem>(entity1);
    TransformSystem& transformSystem2 = gameManager->AddSystem<TransformSystem>(entity2);
    TransformSystem& transformSystem3 = gameManager->AddSystem<TransformSystem>(entity3);

	//transformSystem.AddChildren(b);

    gameManager->Update();
    Transform& retrievedTransform1 = gameManager->GetComponant<Transform>(entity1);
    Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(entity2);
    Transform& retrievedTransform3 = gameManager->GetComponant<Transform>(entity3);


        // Test pour voir si ça marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransformfeu1.position.y << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
        std::cout << "Position X : " << retrievedTransform3.position.x << std::endl;


    return 0;
}