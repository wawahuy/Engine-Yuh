#pragma once
#include "Body.h"
#include "../Contact/ContactManager.h"

S_NS_PHYSICAL
;

/// Lớp vẽ gỡ lỗi
class IDrawDebug {
public:
	virtual void DrawCircle(const Vec2f& position, float radius, float orient) = 0;
	virtual void DrawLine(const Vec2f& pA, const Vec2f& pB) = 0;
	virtual void DrawArrow(const Vec2f& p, const Vec2f& n, float length) = 0;
	virtual void DrawAABB(const Vec2f& min, const Vec2f& max) = 0;
	virtual void DrawPoint(const Vec2f& p) = 0;
	virtual void SetColor(int color) = 0;

	bool active_PointContact  = true;
	bool active_NormalContact = true;
	bool active_AABBDynamic   = false;
	bool active_Velocity	  = true;
};


/// Lớp quản ý tấc cả các đối tượng vật lý trong thế giới
class YUH_API_PHYSICAL World {
	friend class ContactManager;
	friend class Body;

public:
	World(const Vec2f& gravity);

	void Step(float dt, float interationVeclocity, float interationPosition);

	Body* CreateBody();
	void  DestroyBody(Body* body);

	void  SetGravity(const Vec2f& gravity);
	Vec2f GetGravity();

	void SetDrawDebug(IDrawDebug* debug);
	void DrawDebug(const AABB& clip);

	ContactManager* GetContactManager();
	size_t	GetNumBodies();

private:
	ContactManager m_contact_manager;

	IDrawDebug* m_drawDebug;

	Vec2f m_gravity;

	size_t m_num_bodies;
	Body* m_body_begin;
	Body* m_body_end;
};

inline ContactManager* World::GetContactManager() {
	return &m_contact_manager;
}

inline size_t World::GetNumBodies()
{
	return m_num_bodies;
}

E_NS