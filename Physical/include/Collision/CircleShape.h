#pragma once
#include "ICollider.h"

S_NS_PHYSICAL
;


class YUH_API_PHYSICAL CircleShape : public ICollider {
	friend class World;

public:
	CircleShape();

	AABB getAABB();

	bool collide(ICollider* B, Manifold& mf);

	void  SetLocalPosition(const Vec2f& p);
	Vec2f GetLocalPosition();

	void  SetRadius(float r);
	float GetRadius();
	

private:

	float m_radius;
	Vec2f m_position;
};

inline void CircleShape::SetLocalPosition(const Vec2f & p)
{
	m_isChange = true;
	m_position = p;
}

inline void CircleShape::SetRadius(float r)
{
	m_isChange = true;
	m_radius = r;
}

inline float CircleShape::GetRadius() {
	return m_radius;
}

inline Vec2f CircleShape::GetLocalPosition()
{
	return m_position;
}

E_NS