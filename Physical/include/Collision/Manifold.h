#pragma once
#include "../Export.h"
#include "Vector2D.h"

S_NS_PHYSICAL
;

class ICollider;
class Contact;


/// Thông tin va chạm
struct Manifold 
{
	ICollider *colliderA;
	ICollider *colliderB;
	Contact	  *c;

	Vec2f	contact[MANIFOLD_NUM_MAX];
	size_t	contact_count;
	float	penetration;
	Vec2f	normal;
};



E_NS