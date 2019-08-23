#pragma once
#include "AABB.h"
#include "AVLTree.h"

S_NS_PHYSICAL

class Contact;
class ICollider;

/// Thông tin va chạm
struct Manifold {
	Vec2f	contact[2];
	size_t	contact_count;
	float	penetration;
	Vec2f	normal;
};


/// Kết nối tiếp xúc trên Collider A với Collider B
struct ContactConnect {
	ICollider *other;
	Contact	  *contact;
};

inline bool operator==(const ContactConnect& c1, const ContactConnect& c2) {
	return c1.other == c2.other;
}

inline bool operator<(const ContactConnect& c1, const ContactConnect& c2) {
	return c1.other < c2.other;
}



/// Interface
/// Các đối tượng có thể va chạm cần được kế thừa ICollider
/// Là một thành phần trong Broadphase
/// Nó bao gồm việc xây dựng getAABB() quanh đối tượng, và getVelocity() là vận tốc đối tượng
class ICollider {
	friend class Broadphase;
	friend class ContactManager;

public:
	virtual AABB  getAABB() = 0;
	virtual Vec2f getVelocity() = 0;
	virtual bool  collide(ICollider* colider, Manifold& manifold) = 0;

private:
	/// Danh sách tiếp xúc với ICollider còn lại
	AVLTree<ContactConnect> m_listContact;

	/// Chỉ số node trên Broadphase
	int m_nodeIndex;
};


typedef Pair<ICollider *, ICollider *>	IColliderPair;

E_NS