#include "../include/AABB.h"

S_NS_PHYSICAL

bool OverlapAABBAABB(const AABB & aabb1, const AABB & aabb2)
{
	if (aabb1.max.x < aabb2.min.x || aabb2.max.x < aabb1.min.x)
		return false;

	if (aabb1.max.y < aabb2.min.y || aabb2.max.y < aabb1.min.y)
		return false;
	
	return true;
}

bool YUH_API_PHYSICAL ContainsAABBAABB(const AABB & aabb1, const AABB & aabb2)
{
	if (aabb1.min.x > aabb2.min.x || aabb1.min.y > aabb2.min.y)
		return false;

	if (aabb1.max.x < aabb2.max.x || aabb1.max.y < aabb2.max.y)
		return false;

	return true;
}


void YUH_API_PHYSICAL CombineAABB(AABB * aabb, const AABB & aabb1, const AABB & aabb2)
{
	aabb->min.x = std::fminf(aabb1.min.x, aabb2.min.x);
	aabb->min.y = std::fminf(aabb1.min.y, aabb2.min.y);
	aabb->max.x = std::fmaxf(aabb1.max.x, aabb2.max.x);
	aabb->max.y = std::fmaxf(aabb1.max.y, aabb2.max.y);
	return void YUH_API_PHYSICAL();
}

float YUH_API_PHYSICAL AreaAABB(const AABB & aabb)
{
	Vec2f size = aabb.max - aabb.min;
	return 2.0f*size.x*size.y;
}



E_NS