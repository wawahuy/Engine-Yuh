#pragma once
#include <vector>
#include "Export.h"
#include "ICollider.h"
#include "Broadphase.h"

S_NS_PHYSICAL
;

/// Lớp quản lý các vụ va chạm
class ContactManager
{
public:
	ContactManager();
	~ContactManager();

	/// Lấy danh sách các vụ va chạm
	const std::vector<IColliderPair>& GetListContact() const;

	/// Lấy broadphase object
	Broadphase* GetBroadphase();


private:
	Broadphase m_broadphase;
	std::vector<IColliderPair> m_collider_pairs;
};

E_NS

