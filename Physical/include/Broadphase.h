#pragma once
/*
	- Broadphase.h
	- Project	:	Engine Yuh
	- Author	:	Nguyen Gia Huy
	- Email		:	Kakahuy99@gmail.com
					Kakahuy104@gmail.com
	- Start		:	04/08/2019
	- Update	:
*/

#include "Export.h"
#include "ICollider.h"

S_NS_PHYSICAL

class YUH_API_PHYSICAL Broadphase
{
public:
	virtual void Add(ICollider* object) = 0;
	virtual void Remove(ICollider* object) = 0;
	virtual void Update() = 0;
	virtual const IColliderPairList& ComputePair() = 0;
	//virtual const IColliderList& Query(const AABB& aabb) = 0;
};

E_NS