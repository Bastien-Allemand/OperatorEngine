#pragma once
class Transform;

struct ComponentTransform
{
public:
	ComponentTransform() = default;

	Transform* worldTransform;

	Transform* localTransform;

	ComponentTransform* parent = nullptr;
	Vector<ComponentTransform*> children;
};

