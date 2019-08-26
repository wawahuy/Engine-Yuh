#pragma once
#include "ICollider.h"

S_NS_PHYSICAL
;


class YUH_API_PHYSICAL CircleShape : public ICollider {
public:
	AABB getAABB();

	bool collide(ICollider* B, Manifold& mf);

	float GetRadius();

private:
	float m_radius;
};

inline float CircleShape::GetRadius() {
	return m_radius;
}

E_NS