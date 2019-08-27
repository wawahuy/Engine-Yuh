#pragma once
#include "../Export.h"
#include <Vector2D.h>

S_NS_PHYSICAL
;

class Tranform {
	friend class Body;

public:
	Tranform();
	void SetIdentity();
	void SetPosition(const Vec2f& position);
	void SetAngle(float angle);
	void Set(const Vec2f& position, float angle);

	Vec2f GetPosition();
	float GetAngle();
	Vec2f GetRotXAxis();
	Vec2f GetRotYAxis();

	Vec2f operator*(const Vec2f& v);

private:
	Vec2f m_position;
	float m_sin;
	float m_cos;

};

inline Tranform::Tranform() {
	SetIdentity();
}

inline void Tranform::SetIdentity()
{
	m_position = Vec2f(0, 0);
	m_sin = 0;
	m_cos = 0;
}

inline void Tranform::SetPosition(const Vec2f & position)
{
	m_position = position;
}

inline void Tranform::SetAngle(float angle)
{
	m_sin = std::sinf(angle);
	m_cos = std::cosf(angle);
}

inline void Tranform::Set(const Vec2f & position, float angle)
{
	SetPosition(position);
	SetAngle(angle);
}

inline Vec2f Tranform::GetPosition()
{
	return m_position;
}

inline float Tranform::GetAngle()
{
	return std::atan2(m_sin, m_cos);
}

inline Vec2f Tranform::GetRotXAxis()
{
	return Vec2f(m_cos, m_sin);
}

inline Vec2f Tranform::GetRotYAxis()
{
	return Vec2f(-m_sin, m_cos);
}

inline Vec2f Tranform::operator*(const Vec2f & v)
{
	Vec2f vOut = m_position;
	vOut.x += v.x*m_cos - v.y*m_sin;
	vOut.y += v.x*m_sin + v.y*m_cos;
	return vOut;
}


E_NS


