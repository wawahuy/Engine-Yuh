﻿#pragma once
#include "../Common/AABB.h"
#include "../Common/Tranform.h"
#include "AVLTree.h"
#include "Manifold.h"

S_NS_PHYSICAL

class Body;
class Contact;
class ICollider;


/// Thông tin khối lượng, moment quán tính
struct MassData {
	float mass;
	float inertia;
	Vec2f center;
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



/// Abstract
/// Các đối tượng có thể va chạm cần được kế thừa ICollider
/// Là một thành phần trong Broadphase
/// Nó bao gồm việc xây dựng getAABB() quanh đối tượng, và getVelocity() là vận tốc đối tượng
class ICollider {
	friend class Body;
	friend class Broadphase;
	friend class ContactManager;
	friend class CircleShape;
	friend class World;

public:
	ICollider();

	enum Type {
		c_Circle,
		c_Polygon
	};

	virtual AABB  getAABB() = 0;
	virtual bool  collide(ICollider* colider, Manifold& manifold) = 0;
	virtual void  computeMass(MassData* massdata) = 0;

	
	Body* getBody();
	Type  getType();

	void SetDensity(float density);
	void SetRestitution(float restitution);
	void SetFriction(float friction);

protected:
	bool    m_isChange;
	float	m_density;
	float	m_restitution;
	float	m_friction;


private:
	Body *m_body;
	Type  m_type;

	/// Danh sách tiếp xúc với ICollider còn lại
	AVLTree<ContactConnect> m_listContact;

	/// Chỉ số node trên Broadphase
	int m_nodeIndex;

	/// List collider
	ICollider* m_prev;
	ICollider* m_next;
};


inline ICollider::ICollider()
{
	m_restitution = 0;
	m_friction = 0;
	m_density = 0;
	m_isChange = false;
}



inline Body* ICollider::getBody() {
	return m_body;
}

inline ICollider::Type  ICollider::getType() {
	return m_type;
}

inline void ICollider::SetDensity(float density)
{
	m_density = density;
}

inline void ICollider::SetRestitution(float restitution)
{
	m_restitution = restitution;
}

inline void ICollider::SetFriction(float friction)
{
	m_friction = friction;
}


E_NS