#pragma once
#include "System.h"

class TransformSystem : public ISystem
{
public:
	TransformSystem() = default;
	void Update() override;
private:
};

