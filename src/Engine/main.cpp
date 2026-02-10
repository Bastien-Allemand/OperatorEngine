#include "pch.h"
#include "main.h"
#include "GameManager.h"
#include "Entity.h"
// Assurez-vous que Transform est défini ou inclus

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    GameManager* gameManager = GameManager::GetInstance();

    Entity entity1;
    entity1.id = std::make_pair(0, 1);
	gameManager->AddEntity(entity1.id.first, true, true);
    Entity entity2;
    entity2.id = std::make_pair(1, 1);
    gameManager->AddEntity(entity2.id.first, true, true);
    Entity entity3;
    entity3.id = std::make_pair(2, 1);
    gameManager->AddEntity(entity3.id.first, true, true);

    Transform&  a = gameManager->AddComponant<Transform>(entity1.id.first);

	a.position = Vector3f(1.0f, 2.0f, 3.0f);

    Transform& b = gameManager->AddComponant<Transform>(entity2.id.first);
    b.position = Vector3f(70.0f, 9.0f, -58.0f);

    TransformSystem& transformSystem = gameManager->AddSystem<TransformSystem>(entity1.id.first);
    TransformSystem& transformSystem2 = gameManager->AddSystem<TransformSystem>(entity2.id.first);

	//transformSystem.AddEntity(entity1.id.first);
	//transformSystem.AddEntity(entity2.id.first);
	//transformSystem.AddEntity(entity3.id.first);

	//transformSystem.Update( 0.016f); // Simule une mise à jour avec un deltaTime de 16ms
    // 4. Récupération du composant

    Transform& retrievedTransform1 = gameManager->GetComponant<Transform>(entity1.id.first);


    //feu& retrievedTransformfeu1 = gameManager->GetComponant<feu>(entity1.id.first);

    Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(entity2.id.first);
    //feu& retrievedTransformfeu2 = gameManager->GetComponant<feu>(entity2.id.first);

    TransformSystem retrievedSystem1;

    retrievedSystem1.Update({ entity1.id.first, entity2.id.first }, 0.016f);

    try {
       
        //Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(dhgfheis.id.first);
   //     for (auto& system : gameManager->GetSystems())
   //     {
			//system->Update(0.016f);
   //     }


        // Test pour voir si ça marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransformfeu1.position.y << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
       //std::cout << "Position X : " << retrievedTransformfeu2.position.y << std::endl;
        //std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
    }
    catch (const std::exception& e) {
        // Gère le cas où l'entité n'a pas ce composant
        std::cerr << e.what() << std::endl;
    }

    return 0;
}