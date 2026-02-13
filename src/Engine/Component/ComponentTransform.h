#pragma once
class Transform;
#include "Component.h"

struct ComponentTransform : public Component
{
public:
	ComponentTransform() = default;

	Transform* worldTransform;

	Transform* localTransform;

	ComponentTransform* parent = nullptr;
	Vector<ComponentTransform*> children;
};

