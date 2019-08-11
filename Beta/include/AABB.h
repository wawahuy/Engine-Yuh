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
#include <Vector3D.h>

S_NS_PHYSICAL

struct AABB {
	Vec2f min;
	Vec2f max;
};

bool YUH_API_PHYSICAL OverlapAABBAABB(const AABB& aabb1, const AABB& aabb2); 
bool YUH_API_PHYSICAL ContainsAABBAABB(const AABB& aabb1, const AABB& aabb2);
void YUH_API_PHYSICAL CombineAABB(AABB* aabb, const AABB& aabb1, const AABB& aabb2);
float YUH_API_PHYSICAL AreaAABB(const AABB& aabb);

E_NS


