#include "pch.h"
#include "TransformComponent.h"

void TransformComponent::MoveTo(Vector3f _target, float _speed)
{
	m_targetPos = _target;
	m_moveSpeed = _speed;
}
