#pragma once
class Component;

struct Id
{
	uint64 index;
	uint32 componentCount;

	bool operator==(const Id& other) const 
	{
		if (index == other.index && componentCount == other.componentCount)
			return true;

		return false;
	}
};

class Entity
{
public:
	Id id;

	std::vector<Component*> componants;

};
