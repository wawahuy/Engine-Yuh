#pragma once
#include <map>
#include <list>
#include <vector>
#include "AABB.h"

S_NS_PHYSICAL

class ICollider {
	friend class Broadphase;

public:
	virtual void getAABB(AABB* aabb) = 0;
	virtual Vec2f getVelocity() = 0;
};

typedef std::pair<ICollider*, ICollider *> IColliderPair;
typedef std::list<IColliderPair> IColliderPairList;
typedef std::list<ICollider* > IColliderList;

E_NS