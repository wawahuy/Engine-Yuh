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


E_NS


