#include "pch.h"
#include "Geometry.h"

void Geometry::BuildQuad()
{
    // 4 Sommets pour les 4 coins
    vertices = {
        // Position              // Normale (Couleur)    // UV
        {{-0.5f,  0.5f, 0.0f},  {1.0f, 0.0f, 0.0f},     {0.0f, 0.0f}}, // 0 : Haut-Gauche (Rouge)
        {{ 0.5f,  0.5f, 0.0f},  {0.0f, 1.0f, 0.0f},     {1.0f, 0.0f}}, // 1 : Haut-Droite (Vert)
        {{ 0.5f, -0.5f, 0.0f},  {0.0f, 0.0f, 1.0f},     {1.0f, 1.0f}}, // 2 : Bas-Droite  (Bleu)
        {{-0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 0.0f},     {0.0f, 1.0f}}  // 3 : Bas-Gauche  (Jaune)
    };

    // Ordre Clockwise pour DirectX
    indices = {
        0, 1, 2, // Premier triangle (H-G, H-D, B-D)
        0, 2, 3  // Deuxième triangle (H-G, B-D, B-G)
    };
}

void Geometry::BuildVertexBuffer()
{

}