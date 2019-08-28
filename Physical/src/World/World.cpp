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

	dt /= 1000;

	for (auto body = m_body_begin; body; body = body->m_next) {
		for (auto collider = body->m_collider_begin; collider; collider = collider->m_next) {
			
			if (collider->m_isChange || body->m_isChange) {
				m_contact_manager.m_broadphase.Move(collider);
				collider->m_isChange = false;
			}
		}

		body->m_isChange = false;
	}

	for (auto contact = m_contact_manager.m_contact_begin; contact; contact = contact->m_next) {
		const Manifold& mf = contact->m_manifold;
		if (mf.contact_count == 0)
			continue;

		ICollider* cA = contact->m_colliderA;
		ICollider* cB = contact->m_colliderB;
		Body* bA = cA->m_body;
		Body* bB = cB->m_body;

		Vec2f rv = bB->m_linearVelocity - bA->m_linearVelocity;
		float velAlongNormal = rv*mf.normal;
		if (velAlongNormal > 0)
			continue;

		float e = min(cA->m_restitution, cB->m_restitution);

		if (!(bA->m_invMass || bB->m_invMass))
			continue;

		float j = -(1 + e) * velAlongNormal;
		j /= bA->m_invMass + bB->m_invMass;

		Vec2f impulse = mf.normal * j;
		bA->m_linearVelocity -= bA->m_invMass * impulse;
		bB->m_linearVelocity += bB->m_invMass * impulse;
	}

	for (auto contact = m_contact_manager.m_contact_begin; contact; contact = contact->m_next) {
		const Manifold& mf = contact->m_manifold;
		if (mf.contact_count == 0)
			continue;

		ICollider* cA = contact->m_colliderA;
		ICollider* cB = contact->m_colliderB;
		Body* bA = cA->m_body;
		Body* bB = cB->m_body;

		if (!(bA->m_invMass && bB->m_invMass))
			continue;

		const float percent = 0.2; // usually 20% to 80%
		const float slop = 0.01; // usually 0.01 to 0.1
		Vec2f correction = (yuh::max(mf.penetration - slop, 0.0f) / (bA->m_invMass + bB->m_invMass)) * percent * mf.normal;
		bA->m_tfx.m_position -= bA->m_invMass * correction;
		bB->m_tfx.m_position += bB->m_invMass * correction;
	}

	for (auto body = m_body_begin; body; body = body->m_next) {
		for (auto collider = body->m_collider_begin; collider; collider = collider->m_next) {
		}

		if (body->m_type == Body::b_Static)
			continue;

		body->m_linearVelocity += m_gravity*dt*interationVeclocity;
		body->m_tfx.m_position += body->m_linearVelocity*dt*interationPosition;
		body->m_isChange = true;
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

	AVLNode<ContactConnect>* stack[256];
	size_t stack_count;

	for (auto collider : list_collider) {

		/// Draw veclocity
		Body* body = collider->m_body;
		float lvel = VectorLength(body->m_linearVelocity);
		m_drawDebug->DrawArrow(body->m_tfx.m_position, body->m_linearVelocity / lvel, lvel);


		/// Shape
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

		stack_count = 0;
		stack[stack_count++] = collider->m_listContact.GetRoot();
		while (stack_count) {
			auto node = stack[--stack_count];
			if (node) {
				stack[stack_count++] = node->left;
				stack[stack_count++] = node->right;

				Contact* contact = node->data.contact;
				const Manifold& mf = contact->m_manifold;

				if (mf.contact_count == 0)
					continue;

				for (int i = 0; i < mf.contact_count; i++) {
					const Vec2f& c = mf.contact[i];
					m_drawDebug->DrawArrow(c, mf.normal, mf.penetration);
				}

			}
		}

	}

	for (auto aabb : list_aabb) {
		m_drawDebug->DrawAABB(aabb->min, aabb->max);
	}


}


E_NS
