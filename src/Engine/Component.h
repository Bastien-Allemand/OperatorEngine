#pragma once

struct Component
{
    enum class ComponentType
    {
        Transform,
        Mesh,
        Input,
	};
	ComponentType type;
};
