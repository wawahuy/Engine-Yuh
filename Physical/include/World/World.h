#pragma once
#include "Body.h"
#include "../Contact/ContactManager.h"

S_NS_PHYSICAL
;

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

	ContactManager* GetContactManager();

private:
	ContactManager m_contact_manager;

	Vec2f m_gravity;

	Body* m_body_begin;
	Body* m_body_end;
};

inline ContactManager* World::GetContactManager() {
	return &m_contact_manager;
}

E_NS