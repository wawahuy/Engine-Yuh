#pragma once
#include "AABB.h"

S_NS_PHYSICAL

/// Interface
/// Các đối tượng có thể va chạm cần được kế thừa ICollider
/// Là một thành phần trong Broadphase
/// Nó bao gồm việc xây dựng getAABB() quanh đối tượng, và getVelocity() là vận tốc đối tượng
class ICollider {
	friend class Broadphase;

public:
	virtual AABB  getAABB() = 0;
	virtual Vec2f getVelocity() = 0;

private:
	/// Chỉ số node trên Broadphase
	int nodeIndex;
};


typedef Pair<ICollider *, ICollider *>	IColliderPair;

E_NS