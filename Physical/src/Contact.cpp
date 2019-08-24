#include "..\include\Contact.h"

S_NS_PHYSICAL
;


Contact * Contact::GetNext()
{
	return m_next;
}

ICollider * Contact::GetColliderA()
{
	return m_colliderA;
}

ICollider * Contact::GetColliderB()
{
	return m_colliderB;
}

Contact::Contact(ICollider * A, ICollider * B)
{
	m_colliderA = A;
	m_colliderB = B;
	m_prev = NULL;
	m_next = NULL;
	m_flag = 0x0;
}

Contact* Contact::Create(ICollider * A, ICollider * B)
{
	return new Contact(A, B);
}

void Contact::Destroy(Contact * contact)
{
	delete contact;
}




E_NS