#include "../../include/World/World.h"

S_NS_PHYSICAL
;

World::World(const Vec2f& gravity)
{
	m_gravity = gravity;
	m_body_begin = NULL;
	m_body_end = NULL;
}

void World::Step(float dt, float interationVeclocity, float interationPosition)
{
	m_contact_manager.FindNewPair();
	m_contact_manager.Collide();
}

Body * World::CreateBody()
{
	Body* body = new Body();
	body->m_world = this;
	body->m_next = NULL;
	
	if (m_body_end) {
		m_body_end->m_next = body;
		body->m_prev = m_body_end;
	}
	else {
		m_body_begin = body;
	}

	m_body_end = body;

	return body;
}

void World::DestroyBody(Body * body)
{
	/// Đặt lại liên kết
	if (body->m_prev) {
		body->m_prev->m_next = body->m_next;
	}
	else {
		m_body_begin = body->m_next;
	}

	if (body->m_next) {
		body->m_next->m_prev = body->m_prev;
	}
	else {
		m_body_end = body->m_prev;
	}

	/// Xóa tấc cả Collider
	body->Free();

	delete body;
}

void World::SetGravity(const Vec2f& gravity)
{
	m_gravity = gravity;
}

Vec2f World::GetGravity()
{
	return m_gravity;
}


E_NS
