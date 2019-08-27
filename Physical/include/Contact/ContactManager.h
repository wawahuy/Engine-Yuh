#pragma once
#include <vector>
#include "../Export.h"
#include "../World/Broadphase.h"
#include "Contact.h"

S_NS_PHYSICAL
;

/// Interface
/// Lớp lắng nghe sự kiện tiếp xúc
class YUH_API_PHYSICAL ContactListener {
public:
	virtual void BeginContact(Contact* contact) = 0;
	virtual void EndContact(Contact* contact) = 0;
};


/// Lớp quản lý các vụ tiếp xúc
/// Các Contact được lưu lại và được thêm mới thông qua FindNewPair
/// Các Contact được loại bỏ thông qua Collide
class YUH_API_PHYSICAL ContactManager
{
	friend class Body;
	friend class World;

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
	/// Narrow-phase
	void Collide();

	/// Danh sách các vụ tiếp xúc
	Contact*	GetContact();

	/// Lấy broadphase object
	Broadphase* GetBroadphase();

	/// Số lượng contacts
	size_t		GetNumContact();

	/// Đặt sự kiện
	void		SetListener(ContactListener* cb);

	void Free();

private:
	/// Pha rộng
	Broadphase	m_broadphase;

	/// Danh sách vụ tiếp xúc
	Contact*	m_contact_end;
	Contact*	m_contact_begin;
	size_t		m_contact_num;

	/// Sự kiện va chạm
	ContactListener* m_listener;
};

inline Contact * ContactManager::GetContact()
{
	return m_contact_begin;
}


inline Broadphase * ContactManager::GetBroadphase()
{
	return &m_broadphase;
}

inline size_t ContactManager::GetNumContact() {
	return m_contact_num;
}


E_NS

