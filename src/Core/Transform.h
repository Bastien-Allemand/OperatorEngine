#pragma once
#include "Define.h"

struct Transform
{
	Vector3f position;

	Vector3f scale;

	Vector3f direction;
	Vector3f up;
	Vector3f right;
	Vector3f forward;

	Quaternion quaternion;
	Matrix4x4f rot;
};