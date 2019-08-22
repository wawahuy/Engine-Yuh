#pragma once
#include <vector>
#include "Export.h"
#include "Broadphase.h"
#include "Contact.h"

S_NS_PHYSICAL
;

/// Lớp quản lý các vụ tiếp xúc
/// Các Contact được lưu lại và được thêm mới thông qua FindNewPair
/// Các Contact được loại bỏ thông qua Collide
class ContactManager
{
public:
	ContactManager();
	~ContactManager();

	/// Thêm vụ tiếp xúc mới
	void Add(ICollider *A, ICollider *B);

	/// Xóa các tiếp xúc
	void Destroy(Contact *contact);

	/// Tìm kiếm các vụ tiếp xúc mới
	void FindNewPair();

	/// Tìm kiếm các vụ tiếp xúc rõ nhất, giải phóng các vụ tiếp xúc cũ
	/// Middle-phase
	void Collide();

	/// Danh sách các vụ tiếp xúc
	Contact*	GetContact();

	/// Lấy broadphase object
	Broadphase* GetBroadphase();


private:
	/// Pha rộng
	Broadphase	m_broadphase;

	/// Danh sách vụ tiếp xúc
	Contact*	m_contacts;
};

E_NS

