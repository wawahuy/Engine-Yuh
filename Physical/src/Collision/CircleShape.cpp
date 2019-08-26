#include "../../include/Collision/CircleShape.h"
#include "../../include/World/Body.h"

S_NS_PHYSICAL
;

AABB CircleShape::getAABB()
{
	yuh::physical::AABB aabb;
	aabb.max = m_body->m_position + yuh::Vec2f(m_radius, m_radius);
	aabb.min = m_body->m_position - yuh::Vec2f(m_radius, m_radius);
	return aabb;
}

bool CircleShape::collide(ICollider * B, Manifold & mf)
{
	Vec2f posA = m_body->m_position;
	Vec2f posB = B->m_body->m_position;
	float rA = m_radius;
	float rB = ((CircleShape*)B)->m_radius;

	Vec2f normal = posB - posA;
	float distanceSquare = normal*normal;
	float radius = rA + rB;

	if (distanceSquare > radius*radius)
		return false;

	mf.contact_count = 1;
	float distance = sqrt(distanceSquare);
	normal /= distance;

	if (distance == 0) {
		mf.normal = yuh::Vec2f(1, 0);
		mf.penetration = rA;
		mf.contact[0] = posA;
	}
	else {
		mf.normal = normal;
		mf.penetration = radius - distance;
		mf.contact[0] = normal*rA + posA;
	}

	return true;
}


E_NS

