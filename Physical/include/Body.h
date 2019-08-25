#pragma once
#include "Export.h"
#include "ICollider.h"

S_NS_PHYSICAL
;

/// Mỗi body đại diện cho một cơ thể cứng nhắc
/// Một body có thể chứa nhiều Collider
class YUH_API_PHYSICAL Body {
	friend class ContactManager;

public:
	enum Type {
		b_Static,
		b_Dynamic
	};

	/// Khởi tạo các thành phần va chạm trong Body
	ICollider* CreateCollider(ICollider::Type type);


private:
	Body();

	Vec2f	m_position;
	float	m_orient;		// quay (radian)

	Vec2f	m_veclocity;
	Vec2f	m_force;
	float	m_torque;

	float	m_mass;
	float	m_inv_mass;		// nghịch đảo khối lượng

	float	m_restitution;
	float	m_staticFriction;
	float	m_dynamicFriction;
};

E_NS