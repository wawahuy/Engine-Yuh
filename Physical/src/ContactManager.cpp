#include "../include/ContactManager.h"


S_NS_PHYSICAL
;

ContactManager::ContactManager()
{
	m_contact_begin = NULL;
	m_contact_end = NULL;
	m_contact_num = 0;
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
		int iA = contact->m_colliderA->m_nodeIndex;
		int iB = contact->m_colliderB->m_nodeIndex;

		if (m_broadphase.TestOverlap(iA, iB) == false) {
			Contact* tmpDel = contact;
			contact = contact->m_next;
			Destroy(tmpDel);
			continue;
		}

		contact = contact->m_next;
	}
}

E_NS
