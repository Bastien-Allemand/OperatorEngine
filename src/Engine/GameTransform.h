#pragma once
#include "System.h"

class TransformSystem : public System
{
public:
    TransformSystem() = default;
    void Update(float deltaTime) override;
    void AddChildren(Entity* child, Entity* parent);

    void Move();
};

