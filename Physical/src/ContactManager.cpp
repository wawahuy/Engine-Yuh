#include "../include/ContactManager.h"


S_NS_PHYSICAL
;

ContactManager::ContactManager()
{
	m_contacts = NULL;
}


ContactManager::~ContactManager()
{
}

void ContactManager::FindNewPair()
{

}

void ContactManager::Add(ICollider * A, ICollider * B)
{
}

void ContactManager::Destroy(Contact * contact)
{
}

void ContactManager::Collide()
{

}

Contact * ContactManager::GetContact()
{
	return m_contacts;
}


Broadphase * ContactManager::GetBroadphase()
{
	return &m_broadphase;
}

E_NS
