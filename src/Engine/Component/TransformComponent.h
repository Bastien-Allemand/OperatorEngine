#pragma once
#include "Transform.h"

struct TransformComponent
{
public:
	Transform worldTransform;

	Transform localTransform;

	TransformComponent* parent = nullptr;
	Vector<TransformComponent*> children;

	void MoveTo(Vector3f _target,float _speed);
private:
	Vector3f m_targetPos;
	float m_moveSpeed;
};

