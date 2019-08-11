#include "../include/AABB.h"

S_NS_PHYSICAL

AABB AABB::Combine(const AABB & aabb)
{
	AABB aabbCombine;
	aabbCombine.min.x = yuh::min(min.x, aabb.min.x);
	aabbCombine.min.y = yuh::min(min.y, aabb.min.y);
	aabbCombine.max.x = yuh::max(max.x, aabb.max.x);
	aabbCombine.max.y = yuh::max(max.y, aabb.max.y);
	return aabbCombine;
}

bool AABB::Overlap(const AABB & aabb)
{
	if (max.x < aabb.min.x || aabb.max.x < min.x) return false;
	if (max.y < aabb.min.y || aabb.max.y < min.y) return false;
	return true;
}

bool AABB::Contains(const AABB & aabb)
{
	if (min.x > aabb.min.x || min.y > aabb.min.y) return false;
	if (max.x < aabb.max.x || max.y < aabb.max.y) return false;
	return true;
}

float AABB::Area()
{
	return (max.x - min.x)*(max.y - min.y)*2.0f;
}

E_NS
