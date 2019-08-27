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

	for (auto body = m_body_begin; body; body = body->m_next) {
		for (auto collider = body->m_collider_begin; collider; collider = collider->m_next) {

			/// Change when set
			if (collider->m_isChange || body->m_isChange) {
				m_contact_manager.m_broadphase.Move(collider);
				collider->m_isChange = false;
			}

		}

		body->m_isChange = false;
	}
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

void World::SetDrawDebug(IDrawDebug * debug)
{
	m_drawDebug = debug;
}

void World::DrawDebug(const AABB& clip)
{
	Broadphase *bp = m_contact_manager.GetBroadphase();

	std::vector<ICollider *> list_collider;
	std::vector<AABB *> list_aabb;
	bp->QueryAllAABB(clip, list_aabb, list_collider);

	for (auto collider : list_collider) {
		switch (collider->m_type)
		{
		case ICollider::c_Circle: 
		{
			CircleShape* c = (CircleShape*)collider;
			Vec2f p = c->m_body->m_tfx*c->m_position;
			m_drawDebug->DrawCircle(p, c->m_radius);
			break;
		}

		case ICollider::c_Polygon:
			break;
		}
	}


}


E_NS
