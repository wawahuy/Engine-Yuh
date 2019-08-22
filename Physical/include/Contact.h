#pragma once
#include "Export.h"
#include "ICollider.h"


S_NS_PHYSICAL
;

class Contact
{
	friend class ContactManager;

public:
	/// Cặp đối tượng va chạm
	ICollider*	GetColliderA();
	ICollider*	GetColliderB();

	/// Va chạm kế tiếp
	Contact*	GetNext();


private:
	Contact(ICollider * A, ICollider * B);

	static Contact* Create(ICollider* A, ICollider* B);
	static void		Destroy(Contact* contact);

	Contact* m_prev;
	Contact* m_next;

	ICollider* m_colliderA;
	ICollider* m_colliderB;
};

E_NS

