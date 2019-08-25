#pragma once
/*
	- AABB.h
	- Project	:	Engine Yuh
	- Author	:	Nguyen Gia Huy
	- Email		:	Kakahuy99@gmail.com
					Kakahuy104@gmail.com
	- Start		:	04/08/2019
	- Update	:
*/


#include "Export.h"
#include <YMath.h>
#include <Vector3D.h>

S_NS_PHYSICAL

class YUH_API_PHYSICAL AABB {
public:
	Vec2f min;
	Vec2f max;

	AABB  Combine(const AABB& aabb);
	bool  Overlap(const AABB& aabb) const;
	bool  Contains(const AABB& aabb) const;
	float Area();
};

inline float AABB::Area()
{
	return (max.x - min.x)*(max.y - min.y);
}

inline AABB AABB::Combine(const AABB & aabb)
{
	AABB aabbCombine;
	aabbCombine.min.x = yuh::min(min.x, aabb.min.x);
	aabbCombine.min.y = yuh::min(min.y, aabb.min.y);
	aabbCombine.max.x = yuh::max(max.x, aabb.max.x);
	aabbCombine.max.y = yuh::max(max.y, aabb.max.y);
	return aabbCombine;
}

inline bool AABB::Overlap(const AABB & aabb) const
{
	if (max.x < aabb.min.x || aabb.max.x < min.x) return false;
	if (max.y < aabb.min.y || aabb.max.y < min.y) return false;
	return true;
}

inline bool AABB::Contains(const AABB & aabb) const
{
	if (min.x > aabb.min.x || min.y > aabb.min.y) return false;
	if (max.x < aabb.max.x || max.y < aabb.max.y) return false;
	return true;
}

E_NS


