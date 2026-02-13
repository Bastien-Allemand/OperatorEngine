#pragma once
class Transform;
#include "Component.h"

struct TransformComponent : public Component
{
public:
	TransformComponent()
	{
		type = ComponentType::Transform;
	}

	Transform* worldTransform;

	Transform* localTransform;

	TransformComponent* parent = nullptr;
	Vector<TransformComponent*> children;
};

