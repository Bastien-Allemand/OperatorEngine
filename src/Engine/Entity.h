#pragma once
#include <iostream>
#include <vector>

#define INDEX int
#define NUMBER_OF_COMPONENTS int
struct Entity
{
	std::pair<INDEX, NUMBER_OF_COMPONENTS> id;
};