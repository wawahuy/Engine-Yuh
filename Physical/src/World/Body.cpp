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



E_NS
