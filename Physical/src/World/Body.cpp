#include "../../include/World/Body.h"
#include "../../include/World/World.h"

S_NS_PHYSICAL
;


ICollider * Body::CreateCollider(ICollider::Type type)
{
	ICollider *collider;

	switch (type)
	{
	case ICollider::c_Circle:
		collider = new CircleShape();
		break;

	default:
		return NULL;
		break;
	}

	collider->m_body = this;
	collider->m_next = NULL;

	if (m_collider_end) {
		m_collider_end->m_next = collider;
		collider->m_prev = m_collider_end;
	}
	else {
		m_collider_begin = collider;
	}

	m_collider_end = collider;

	
	/// Thêm vào broadphase
	m_world->m_contact_manager.m_broadphase.Add(collider);


	return collider;
}

void Body::Destroy(ICollider * collider)
{
	/// Đặt lại liên kết trên body
	if (collider->m_prev) {
		collider->m_prev->m_next = collider->m_next;
	}
	else {
		m_collider_begin = collider->m_next;
	}

	if (collider->m_next) {
		collider->m_next->m_prev = collider->m_prev;
	}
	else {
		m_collider_end = collider->m_prev;
	}

	/// Xóa tấc cả Contact trên manager
	/// Xóa ContactConnect Trên ColliderA và ColliderB
	int	stack_count = 0;
	AVLNode<ContactConnect>* stack[256];
	stack[stack_count++] = collider->m_listContact.GetRoot();

	while (stack_count)
	{
		auto node = stack[--stack_count];
		if (node) {
			stack[stack_count++] = node->left;
			stack[stack_count++] = node->right;

			Contact* contact = node->data.contact;
			ICollider* colliderOther = node->data.other;

			/// Xóa trên ContactManager
			m_world->m_contact_manager.Destroy(contact);

			/// Xóa contact connect trên Collider còn lại
			colliderOther->m_listContact.RemoveValue({ collider });

			delete node;
		}
	}

	/// Xóa khỏi broadphae
	m_world->m_contact_manager.m_broadphase.Remove(collider);

	delete collider;
}

Body::Body()
{
	m_type = b_Dynamic;
	m_active = true;
	m_isChange = false;
	m_collider_begin = NULL;
	m_collider_end = NULL;

	m_linearVelocity = Vec2f(0, 0);
	m_angularVelocity = 0.0f;
	m_force = Vec2f(0, 0);
	m_gravityScale = Vec2f(1, 1);
	m_torque = 0.0f;
	m_mass = 0.0f;
	m_invMass = 0.0f;
	m_inertia = 0.0f;
	m_invInertia = 0.0f;
}

void Body::Free()
{
	ICollider* collider = m_collider_begin;
	int	stack_count = 0;
	AVLNode<ContactConnect>* stack[256];

	while(collider) {
		stack[stack_count++] = collider->m_listContact.GetRoot();

		while (stack_count)
		{
			auto node = stack[--stack_count];
			if (node) {
				stack[stack_count++] = node->left;
				stack[stack_count++] = node->right;

				Contact* contact = node->data.contact;
				ICollider* colliderOther = node->data.other;

				/// Xóa trên ContactManager
				m_world->m_contact_manager.Destroy(contact);

				/// Xóa contact connect trên Collider còn lại
				colliderOther->m_listContact.RemoveValue({ collider });

				delete node;
			}
		}

		ICollider *temp = collider;
		collider = collider->m_next;

		/// Xóa khỏi broadphae
		m_world->m_contact_manager.m_broadphase.Remove(temp);

		delete temp;
	}
}


void Body::ResetDataMass()
{
	m_tfx.m_origin.Reset();
	m_inertia = 0;
	
	MassData massdata;
	for (auto temp = m_collider_begin; temp; temp = temp->m_next) {
		temp->computeMass(&massdata);
		m_mass += massdata.mass;
		m_tfx.m_origin += massdata.center*massdata.mass;
		m_inertia += massdata.inertia;
	}


	if (m_mass) {
		m_invMass = 1 / m_mass;
		m_tfx.m_origin *= m_invMass;
	}
	else {
		m_tfx.m_origin.Reset();
	}


	if (m_inertia > 0) {
		m_inertia -= m_mass*m_tfx.m_origin*m_tfx.m_origin;
		m_invInertia = 1.0f / m_inertia;
	}
	else {
		m_inertia = 0.0f;
		m_invInertia = 0.0f;
	}


	if (m_type == Body::b_Static) {
		m_invMass = 0.0f;
	}
}

E_NS
