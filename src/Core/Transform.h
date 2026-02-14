#pragma once
class Transform
{
public:

	Transform() = default;

	void MoveFoward(float distance);
	void MoveRight(float distance);
	void MoveUp(float distance);

	void AddYPR(float yaw, float pitch, float roll);
	void SetYPR(float yaw, float pitch, float roll);

	Matrix4x4f BuildMatrix() const;

	void SetPosition(float x, float y, float z);
	void SetScale(float x, float y, float z);

	Vector3f position = {0,0,0};

	Vector3f scale = {1,1,1};

	Vector3f up = VEC3_UP;
	Vector3f right = VEC3_RIGHT;
	Vector3f forward = VEC3_FOWARD;
	Vector4f quaternion = QUAT_ZERO;

	Matrix4x4f rotmatrix = MATRIX_IDENTITY;
};

