#include "../include/ContactManager.h"


S_NS_PHYSICAL
;

ContactManager::ContactManager()
{
}


ContactManager::~ContactManager()
{
}

const std::vector<IColliderPair>& ContactManager::GetListContact() const
{
	return m_collider_pairs;
}

Broadphase * ContactManager::GetBroadphase()
{
	return &m_broadphase;
}

E_NS
