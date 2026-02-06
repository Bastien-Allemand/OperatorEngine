//#include "pch.h"
//#include "main.h"
//#include "GameManager.h"
//#include "Entity.h"
//
//int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
//{
//	GameManager* gameManager = GameManager::GetInstance();
//	Entity entity;
//	gameManager->AddComponant<Transform>(0);
//	entity.id = std::make_pair(0, 1);
//	for (int i = 0; i < entity.id.second; i++)
//	{
//		auto componenet = gameManager->GetComponant<>(entity.id[]);
//	}
//	
//    return 0;
//}
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
	gameManager->AddEntity(entity1.id.first);
    Entity entity2;
    entity2.id = std::make_pair(1, 1);
    gameManager->AddEntity(entity2.id.first);
    Entity entity3;
    entity3.id = std::make_pair(2, 1);
    gameManager->AddEntity(entity3.id.first);

    Transform&  a = gameManager->AddComponant<Transform>(entity1.id.first);
    feu& f = gameManager->AddComponant<feu>(entity1.id.first);
    a.position.x = 5.0f;
    a.position.y = 12.0f;

    f.position.x = 1.0f;
    f.position.y = -8.0f;


    Transform& b = gameManager->AddComponant<Transform>(entity2.id.first);
    feu&  feur = gameManager->AddComponant<feu>(entity2.id.first);

    b.position.x = 90.0f;
    b.position.y = 2.0f;

    feur.position.x = 15.0f;
    feur.position.y = -81.0f;


	gameManager->AddComponant<Transform>(entity3.id.first);
    gameManager->AddComponant<feu>(entity3.id.first);


    // 4. Récupération du composant
    try {
        Transform& retrievedTransform1 = gameManager->GetComponant<Transform>(entity1.id.first);
        feu& retrievedTransformfeu1 = gameManager->GetComponant<feu>(entity1.id.first);

        Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(entity2.id.first);
        feu& retrievedTransformfeu2 = gameManager->GetComponant<feu>(entity2.id.first);

        //Transform& retrievedTransform2 = gameManager->GetComponant<Transform>(dhgfheis.id.first);



        // Test pour voir si ça marche
        std::cout << "Position X : " << retrievedTransform1.position.x << std::endl;
        std::cout << "Position X : " << retrievedTransformfeu1.position.x << std::endl;

        std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
        std::cout << "Position X : " << retrievedTransformfeu2.position.x << std::endl;
        //std::cout << "Position X : " << retrievedTransform2.position.x << std::endl;
    }
    catch (const std::exception& e) {
        // Gère le cas où l'entité n'a pas ce composant
        std::cerr << e.what() << std::endl;
    }

    return 0;
}