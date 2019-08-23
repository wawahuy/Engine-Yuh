#pragma once
#include "Export.h"
#include "ICollider.h"


S_NS_PHYSICAL
;


class YUH_API_PHYSICAL Contact
{
	friend class ContactManager;

public:
	/// Cặp đối tượng va chạm
	ICollider*	GetColliderA();
	ICollider*	GetColliderB();

	/// Va chạm kế tiếp
	Contact*	GetNext();
	Contact*	operator++(int);

	/// Thông tin va chạm
	const Manifold& GetManifold() const;


private:
	Contact(ICollider * A, ICollider * B);

	static Contact* Create(ICollider* A, ICollider* B);
	static void		Destroy(Contact* contact);

	Contact* m_prev;
	Contact* m_next;

	ICollider* m_colliderA;
	ICollider* m_colliderB;

	Manifold m_manifold;
};

inline Contact* Contact::operator++(int) {
	return m_next;
}

inline const Manifold& Contact::GetManifold() const {
	return m_manifold;
}

E_NS

