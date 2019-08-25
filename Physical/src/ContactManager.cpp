#include "../include/ContactManager.h"


S_NS_PHYSICAL
;

ContactManager::ContactManager()
{
	m_contact_begin = NULL;
	m_contact_end = NULL;
	m_contact_num = 0;
	m_listener = NULL;
}


ContactManager::~ContactManager()
{
}

void ContactManager::FindNewPair()
{
	m_broadphase.UpdatePair(this);
}

void ContactManager::Add(ICollider * A, ICollider * B)
{
	auto node = A->m_listContact.FindNode({ B });

	if (node && *node) {
		return;
	}

	Contact* contact = Contact::Create(A, B);

	/// Đấy contact vào danh sách tiếp xúc
	if (m_contact_end) {
		m_contact_end->m_next = contact;
		contact->m_prev = m_contact_end;
		m_contact_end = contact;
	}
	else {
		m_contact_end = contact;
		m_contact_begin = contact;
	}

	/// Thêm contact vào danh sách kết nối trên A
	/// Không thêm trên B vì chỉ cần xét trên 1 nhánh
	A->m_listContact.AddValue({B, contact});

	++m_contact_num;
}

void ContactManager::Destroy(Contact * contact)
{
	ICollider *colliderA = contact->m_colliderA;
	ICollider *colliderB = contact->m_colliderB;

	/// Gọi event
	if (contact->IsTouching() && m_listener) {
		m_listener->EndContact(contact);
	}

	/// Xóa contact trên collider A
	colliderA->m_listContact.RemoveValue({ colliderB });

	/// Xóa contact trên danh sách
	if (contact->m_prev) {
		contact->m_prev->m_next = contact->m_next;
	}
	else {
		m_contact_begin = contact->m_next;
	}

	if (contact->m_next) {
		contact->m_next->m_prev = contact->m_prev;
	}
	else {
		m_contact_end = contact->m_prev;
	}

	/// Xóa contact
	Contact::Destroy(contact);

	--m_contact_num;
}

void ContactManager::Collide()
{
	Contact *contact = m_contact_begin;
	while (contact)
	{
		ICollider *colliderA = contact->m_colliderA;
		ICollider *colliderB = contact->m_colliderB;
		int iA = colliderA->m_nodeIndex;
		int iB = colliderB->m_nodeIndex;

		if (m_broadphase.TestOverlap(iA, iB) == false) {
			Contact* tmpDel = contact;
			contact = contact->m_next;
			Destroy(tmpDel);
			continue;
		}

		bool touching = false;
		bool wasTouching = contact->m_flag & Contact::FlagContact::touch;

		/// Kiểm tra narrow-phase
		touching = colliderA->collide(colliderB, contact->m_manifold);

		

		if (touching && wasTouching == false)
		{
			contact->m_manifold.colliderA = colliderA;
			contact->m_manifold.colliderB = colliderB;
			contact->m_manifold.c = contact;

			if (m_listener)
			{
				contact->m_flag |= Contact::FlagContact::touch;
				m_listener->BeginContact(contact);
			}
		}


		if (touching == false && wasTouching && m_listener) {
			contact->m_flag ^= Contact::FlagContact::touch;
			m_listener->EndContact(contact);
		}


		contact = contact->m_next;
	}
}

void ContactManager::SetListener(ContactListener * cb)
{
	m_listener = cb;
}

void ContactManager::Free()
{
	/// Free broadphase
	m_broadphase.Free();

	/// Free contact
	Contact *contact = m_contact_begin;
	while (contact)
	{
		Contact::Destroy(contact);
		contact = contact->m_next;
	}

}

E_NS
