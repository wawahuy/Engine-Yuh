#include "../../include/Collision/CircleShape.h"
#include "../../include/World/Body.h"

S_NS_PHYSICAL
;

CircleShape::CircleShape()
{
	m_position = Vec2f(0, 0);
}

AABB CircleShape::getAABB()
{
	Vec2f p = m_body->m_tfx*m_position;
	AABB aabb;
	aabb.max = p + yuh::Vec2f(m_radius, m_radius);
	aabb.min = p - yuh::Vec2f(m_radius, m_radius);
	return aabb;
}

bool CircleShape::collide(ICollider * B, Manifold & mf)
{
	CircleShape *cirB = (CircleShape*)B;
	Body *bdA = m_body;
	Body *bdB = cirB->m_body;

	Vec2f posA = bdA->m_tfx*m_position;
	Vec2f posB = bdB->m_tfx*cirB->m_position;
	float rA = m_radius;
	float rB = cirB->m_radius;

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

