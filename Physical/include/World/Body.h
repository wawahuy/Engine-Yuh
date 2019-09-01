#pragma once
#include "../Export.h"
#include "../Collision/ICollider.h"
#include "../Collision/CircleShape.h"


S_NS_PHYSICAL
;

class World;

/// Mỗi body đại diện cho một cơ thể cứng nhắc
/// Một body có thể chứa nhiều Collider
class YUH_API_PHYSICAL Body {
	friend class Broadphase;
	friend class ContactManager;
	friend class ICollider;
	friend class World;
	friend class CircleShape;

public:
	enum Type {
		b_Static,
		b_Dynamic
	};

	ICollider* CreateCollider(ICollider::Type type);
	void Destroy(ICollider* collider);

	void SetType(Type type);
	Type GetType();

	void SetActive(bool active);
	bool IsActive();

	void  SetPosition(const Vec2f& p);
	Vec2f GetPosition();

	void  SetOrient(float o);
	float GetOrient();

	void  ResetDataMass();

private:
	Body();

	/// Giải phóng body bao gồm các collider và contact trên collider
	void Free();

	World* m_world;
	
	Type	m_type;
	bool	m_active;

	/// cập nhật trên broadphase
	bool	m_isChange;

	/// Ma trận dịch chuyển
	Tranform m_tfx;

	/// Vận tốc
	Vec2f  m_linearVelocity;
	float  m_angularVelocity;

	Vec2f	m_force;
	float	m_torque;
	float	m_mass;
	float	m_invMass;
	float	m_inertia;
	float	m_invInertia;

	/// List Collider
	ICollider* m_collider_begin;
	ICollider* m_collider_end;

	/// List Body
	Body* m_prev;
	Body* m_next;
};


 
inline Body::Type Body::GetType() {
	return m_type;
}

inline bool Body::IsActive() {
	return m_active;
}

inline void Body::SetPosition(const Vec2f & p)
{
	m_isChange = true;
	m_tfx.m_position = p;
}

inline Vec2f Body::GetPosition()
{
	return m_tfx.m_position;
}

inline void Body::SetOrient(float o)
{
	m_isChange = true;
	m_tfx.SetAngle(o);
}

inline float Body::GetOrient()
{
	return m_tfx.GetAngle();
}


inline void Body::SetActive(bool active) {
	m_active = active;
}

inline void Body::SetType(Body::Type type) {
	m_type = type;
}

E_NS


