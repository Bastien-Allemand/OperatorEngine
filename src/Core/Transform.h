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

	Vector3f position;

	Vector3f scale;

	Vector3f up;
	Vector3f right;
	Vector3f forward;
	Vector4f quaternion;

	Matrix4x4f rotmatrix;
};

