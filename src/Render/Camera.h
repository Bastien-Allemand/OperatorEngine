#pragma once
class Transform;

class Camera
{
public:
	Camera();
	Transform* transform;
	Matrix BuildViewMatrix() const;
};

